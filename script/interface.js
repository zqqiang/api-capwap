// translate head.h to api which build struct into json object
// so that other program can reuse these api to build json interface

const fs = require('fs');
const S = require('string');

function toSource(code) {
    fs.appendFileSync('capwap/wtp/cwJsonApi.c', code, 'utf8', (err) => {
        if (err) throw err;
    });
};

function toHeader(code) {
    fs.appendFileSync('capwap/include/cwJsonApi.h', code, 'utf8', (err) => {
        if (err) throw err;
    });
};

function getNewMethod(type) {
    switch (type) {
        case 'int8_t':
        case 'uint8_t':
        case 'uint16_t':
        case 'uint32_t':
            {
                return 'json_object_new_int';
            }
        default:
            {
                let parts;
                if (parts = type.match(/(\w+)_t/)) {
                    return 'build' + S('-' + parts[1]).camelize().s;
                }
                return 'UnsupportType_' + type;
            }
    }
};

function parseLine(line, context) {
    // skip comment
    let parts = line.match(/\s+\/\/\s+/);
    if (parts) {
        console.log('skip comments: ', parts);
        return;
    }

    // match typedef struct
    parts = line.match(/typedef\s+struct\s+(\w+)\s+{/);
    if (parts) {
        context.type = parts[1];
        context.last = 'start';
        toSource('\r\nstruct json_object* build' + S('-' + context.type).camelize().s + '(struct ' + context.type + ' *' + context.type + ') {\r\n');
        toHeader('struct json_object* build' + S('-' + context.type).camelize().s + '(struct ' + context.type + ' *' + context.type + ');\r\n');
        context.indent = '    ';
    }

    // match type value
    parts = line.match(/\s+(\w+)\s+(\w+);/);
    if (parts) {
        let key = parts[1];
        let value = parts[2];
        if ('start' === context.last) {
            if ('uint16_t' === key && 'cnt' === value) {
                toSource(context.indent + 'struct json_object *array = json_object_new_array();\r\n');
                toSource(context.indent + 'struct ' + context.type + ' *e = ' + context.type + ';\r\n');
                toSource(context.indent + 'while (e) {\r\n');
                context.indent = '        ';
                toSource(context.indent + 'struct json_object *obj = json_object_new_object();\r\n');
                toSource(context.indent + 'json_object_object_add(obj, "' + value + '", ' + getNewMethod(key) + '(' + context.type + '->' + value + '));\r\n');
            } else {
                toSource(context.indent + 'struct json_object *obj = json_object_new_object();\r\n');
                toSource(context.indent + 'json_object_object_add(obj, "' + value + '", ' + getNewMethod(key) + '(' + context.type + '->' + value + '));\r\n');
            }
        } else {
            toSource(context.indent + 'json_object_object_add(obj, "' + value + '", ' + getNewMethod(key) + '(' + context.type + '->' + value + '));\r\n');
        }
        context.last = 'attribute';
    }

    // match type *value
    parts = line.match(/\s+(\w+)\s+\*(\w+);/);
    if (parts) {
        let key = parts[1];
        let value = parts[2];

        if ('start' === context.last) {
            if ('uint16_t' === key && 'cnt' === value) {
                toSource(context.indent + 'struct json_object *array = json_object_new_array();\r\n');
                toSource(context.indent + 'struct ' + context.type + ' *e = ' + context.type + ';\r\n');
                toSource(context.indent + 'while (e) {\r\n');
                context.indent = '      ';
                toSource(context.indent + 'json_object_object_add(obj, "' + value + '", ' + getNewMethod(key) + '(' + context.type + '->' + value + '));\r\n');
            } else {
                toSource(context.indent + 'struct json_object *obj = json_object_new_object();\r\n');
                toSource(context.indent + 'json_object_object_add(obj, "' + value + '", ' + getNewMethod(key) + '(' + context.type + '->' + value + '));\r\n');
            }
        } else if ('next' === value) {
            toSource(context.indent + 'json_object_array_add(array, obj);\r\n');
            toSource(context.indent + 'e = e->next;\r\n');
            context.indent = '    ';
            toSource(context.indent + '}\r\n');
        } else {
            toSource(context.indent + 'json_object_object_add(obj, "' + value + '", ' + getNewMethod(key) + '(' + context.type + '->' + value + '));\r\n');
        }

        context.last = ('next' === value) ? 'next' : 'pointer';
    }

    // match bitset field => type name : number
    parts = line.match(/.*?(\w+)\s+:\s+\d+[,;]/);
    if (parts) {
        let value = parts[1];
        if ('start' === context.last) {
            toSource(context.indent + 'struct json_object *obj = json_object_new_object();\r\n');
        }
        toSource(context.indent + 'json_object_object_add(obj, "' + value + '", json_object_new_int(' + context.type + '->' + value + '));\r\n');
        context.last = 'bitset';
    }

    // match endline => };
    parts = line.match(/}.*?(\w+);/);
    if (parts) {
        if ('next' === context.last) {
            toSource(context.indent + 'return array;\r\n');
        } else {
            toSource(context.indent + 'return obj;\r\n');
        }
        context.indent = '';
        toSource('}\r\n');
        context.last = 'end';
    }
};

let topInterface = [
    'vsp_dot11_mgmt_vap',
    'cw_mac_entry',
    'vsp_dot11_ap_list',
    'vsp_dot11_ap_suppress_list',
    'vsp_dot11_sta_list',
    'vsp_vap_downup',
    'cw_sta_entry_add',
    'cw_sta_entry_del',
    'vsp_wds_sta',
    'cw_wlan_entry',
    'vsp_vap_psk_passwd',
    'vsp_vap_radius_config',
    'vsp_vap_wds',
    'vsp_vap_whitelist',
    'vsp_vap_ip_mask',
    'vsp_wtp_split_tun',
    'vsp_vap_mcast_rate',
    'vsp_vap_sta_max_ap',
    'vsp_vap_rates',
    'vsp_vap_bc_suppression',
    'vsp_vap_flags',
    'vsp_vap_natip_mask',
    'vsp_me_disable_thresh',
    'vsp_prob_resp_suppress',
    'vsp_vap_vlan_tag',
    'vsp_ip_frag',
    'vsp_dot11_ebp',
    'vsp_dot11_aero_enable',
    'vsp_dot11_aero_debug',
    'vsp_dot11_fortipresence_params',
    'vsp_mesh_eth_type',
    'vsp_fiap_eth_type',
    'vsp_sta_locate_reset',
    'vsp_max_retransmit',
    'vsp_lldp_enable',
];

function addTopInterface() {
    toHeader('\r\n');
    topInterface.forEach((intf, index) => {
        toSource('\r\nvoid build' + S('-' + intf).camelize().s + 'Json(struct json_object *params, const char *key, struct ' + intf + ' *' + intf + ') {\r\n');
        toSource('    json_object_object_add(params, key, build' + S('-' + intf).camelize().s + '(' + intf + '));\r\n}\r\n');
        toHeader('void build' + S('-' + intf).camelize().s + 'Json(struct json_object *params, const char *key, struct ' + intf + ' *' + intf + ');\r\n');
    });
};

fs.readFile('./head.h', 'utf8', (err, data) => {
    if (err) throw err;
    let lines = data.match(/[^\r\n]+/g);

    // erase cwJsonApi.c and cwJsonApi.h
    [
        'capwap/wtp/cwJsonApi.c',
        'capwap/include/cwJsonApi.h'
    ].forEach((file) => {
        fs.writeFileSync(file, '\r\n', 'utf8', (err) => {
            if (err) throw err;
        });
    });

    toSource('#include "cwJsonApi.h"\r\n');

    toHeader('#ifndef _CW_JSON_API_H\r\n');
    toHeader('#define _CW_JSON_API_H\r\n\r\n');
    toHeader('#include "cw.h"\r\n');
    toHeader('#include "cwWTP.h"\r\n\r\n');

    let context = {};
    lines.forEach((line, index) => {
        parseLine(line, context);
    });

    addTopInterface();

    toHeader('\r\n#endif /* _CW_JSON_API_H */\r\n');
});
