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

    // match macAddr[];
    parts = line.match(/\s+(\w+)\s+macAddr\[\];/);
    if (parts) {
        toSource(context.indent + 'json_object_object_add(obj, "macAddr", buildArray(' + context.type + '->macAddr, ' + context.type + '->macLen));\r\n');
        context.last = 'attribute';
    }

    // match macAddr[CW_MAC_ADDR_LEN_MAX]
    parts = line.match(/\s+(\w+)\s+macAddr\[CW_MAC_ADDR_LEN_MAX\];/);
    if (parts) {
        toSource(context.indent + 'json_object_object_add(obj, "macAddr", buildArray(' + context.type + '->macAddr, ' + context.type + '->macLen));\r\n');
        context.last = 'attribute';        
    }

    // match macAddr[ETH_ALEN]
    parts = line.match(/\s+(\w+)\s+macAddr\[ETH_ALEN\];/);
    if (parts) {
        if ('start' === context.last) {
            toSource(context.indent + 'struct json_object *obj = json_object_new_object();\r\n');
            toSource(context.indent + 'if (' + context.type + ') {\r\n');
            context.indent = '        ';
            toSource(context.indent + 'json_object_object_add(obj, "macAddr", buildArray(' + context.type + '->macAddr, ETH_ALEN));\r\n');
        } else {
            toSource(context.indent + 'json_object_object_add(obj, "macAddr", buildArray(' + context.type + '->macAddr, ETH_ALEN));\r\n');
        }
        context.last = 'attribute';
    }

    // match key[CW_KEY_LEN_MAX]
    parts = line.match(/\s+(\w+)\s+key\[CW_KEY_LEN_MAX\];/);
    if (parts) {
        toSource(context.indent + 'json_object_object_add(obj, "key", buildArray(' + context.type + '->key, ' + context.type + '->kLen));\r\n');
        context.last = 'attribute';
    }

    // match key value[len]
    parts = line.match(/\s+(\w+)\s+(\w+)\[(\d+)\];/);
    if (parts) {
        let key = parts[1];
        let value = parts[2];
        let len = parts[3];
        if ('0' === len) {
            if ('rates' === value) {
                toSource(context.indent + 'json_object_object_add(obj, "' + value + '", buildArray(' + context.type + '->' + value + ', ' + context.type + '->rate_num));\r\n');
            } else if ('ie' === value) {
                toSource(context.indent + 'json_object_object_add(obj, "' + value + '", buildArray(' + context.type + '->' + value + ', ' + context.type + '->ie_len));\r\n');
            } else if ('key' === value) {
                toSource(context.indent + 'json_object_object_add(obj, "' + value + '", buildArray(' + context.type + '->' + value + ', ' + context.type + '->kLen));\r\n');
            }
        } else {
            if ('sn' === value) {
                toSource(context.indent + 'json_object_object_add(obj, "' + value + '", json_object_new_string(' + context.type + '->' + value + '));\r\n');
            } else if ('slct' === value) {
                toSource(context.indent + 'json_object_object_add(obj, "' + value + '", buildArray(' + context.type + '->' + value + ', ' + context.type + '->cnt));\r\n');
            } else {
                toSource(context.indent + 'json_object_object_add(obj, "' + value + '", buildArray(' + context.type + '->' + value + ', ' + len + '));\r\n');
            }
        }
        context.last = 'attribute';
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
                toSource(context.indent + 'if (' + context.type + ') {\r\n');
                context.indent = '        ';
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
                toSource(context.indent + 'if (' + context.type + ') {\r\n');
                context.indent = '        ';                
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
            toSource(context.indent + 'if (' + context.type + ') {\r\n');
            context.indent = '        ';
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
            context.indent = '    ';
            toSource(context.indent + '}\r\n');
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
    'vsp_dot11_aero_tag',
    'vsp_dot11_aero_mu',
    'vsp_dot11_fortipresence_enable',
    'vsp_vap_bitmap',
    'base_radio_admin_state',
    'dot11_md_cap',
    'dot11_wtp_radio_info',
    'base_vsp_country_code',
    'dot11_wtp_radio_cfg',
    'vsp_dot11_ps_opt',
    'vsp_dot11_tx_opt',
    'vsp_dot11_protmode',
    'vsp_dot11_amsdu',
    'vsp_dot11_coext',
    'vsp_dot11_pure',
    'vsp_dot11_ht_cap',
    'dot11_antenna',
    'dot11_tx_pwr',
    'dot11_dsss_ctl',
    'dot11_ofdm_ctl',
    'dot11_mac_oper',
    'vsp_darrp_cfg',
    'base_vsp_ap_scan',
    'base_vsp_ap_scan_idle',
    'base_vsp_ap_scan_passive',
    'base_vsp_ap_scan_sniffer',
    'base_vsp_sta_scan',
    'vsp_fho_enable',
    'vsp_apho_enable',
    'vsp_sta_locate_cfg',
    'vsp_txpwr_percentage',
    'vsp_spectrum_analysis',
    'vsp_wids_enable',
    'vsp_wids_params_long_dur',
    'vsp_wids_params_auth_time',
    'vsp_wids_params_auth_thresh',
    'vsp_wids_params_assoc_time',
    'vsp_wids_params_assoc_thresh',
    'vsp_wids_params_deauth_thresh',
    'vsp_radio_sta_max',
    'vsp_max_distance',
    'vsp_vap_web_authserver',
    'base_ac_desc',
    'vsp_wtp_allow',
    'vsp_wbh_sta',
    'base_cw_timers',
    'base_decrypt_err_rpt_period',
    'vsp_sta_stats_interval',
    'vsp_timers_interval',
    'vsp_sta_cap_interval',
    'vsp_wtp_led_dark',
    'vsp_wtp_sta_max',
    'base_vsp_telnet_enable',
    'base_vsp_http_enable',
    'base_vsp_https_enable',
    'base_vsp_ssh_enable',
    'vsp_mesh_eth_bridge',
];

function addTopInterface() {
    toHeader('\r\n');
    topInterface.forEach((intf, index) => {
        toSource('\r\nvoid build' + S('-' + intf).camelize().s + 'Json(struct json_object *params, const char *key, struct ' + intf + ' *' + intf + ') {\r\n');
        toSource('    json_object_object_add(params, key, build' + S('-' + intf).camelize().s + '(' + intf + '));\r\n}\r\n');
        toHeader('void build' + S('-' + intf).camelize().s + 'Json(struct json_object *params, const char *key, struct ' + intf + ' *' + intf + ');\r\n');
    });
};

function addToolInterface() {
    toSource('\r\nstruct json_object* buildArray(uint8_t *macAddr, uint8_t macLen) {\r\n');
    toSource('    int i = 0;\r\n');
    toSource('    struct json_object *array = json_object_new_array();\r\n');
    toSource('    for(i = 0; i < macLen; ++i) {\r\n');
    toSource('        json_object_array_add(array, json_object_new_int(macAddr[i]));\r\n');
    toSource('    }\r\n');
    toSource('    return array;\r\n');
    toSource('}\r\n');
};

fs.readFile('./head.h', 'utf8', (err, data) => {
    if (err) throw err;
    let lines = data.match(/[^\r\n]+/g);

    // erase cwJsonApi.c and cwJsonApi.h
    [
        'capwap/wtp/cwJsonApi.c',
        'capwap/include/cwJsonApi.h',
    ].forEach((file) => {
        fs.writeFileSync(file, '\r\n', 'utf8', (err) => {
            if (err) throw err;
        });
    });

    toSource('#include "cwJsonApi.h"\r\n');

    addToolInterface();

    toHeader('#ifndef _CW_JSON_API_H\r\n');
    toHeader('#define _CW_JSON_API_H\r\n\r\n');
    toHeader('#include "cw.h"\r\n');
    toHeader('#include "cwWTP.h"\r\n');
    toHeader('#include "json.h"\r\n\r\n');

    let context = {};
    lines.forEach((line, index) => {
        parseLine(line, context);
    });

    addTopInterface();

    toHeader('\r\n#endif /* _CW_JSON_API_H */\r\n');
});
