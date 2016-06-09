// translate head.h to api which build struct into json object
// so that other program can reuse these api to build json interface

const fs = require('fs');
const S = require('string');

function toCode(code) {
    fs.appendFileSync('api.c', code, 'utf8', (err) => {
        if (err) throw err;
    });
};

function getNewMethod(type) {
    switch (type) {
        case 'uint8_t':
        case 'uint16_t':
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
    // match typedef struct
    let parts = line.match(/typedef\s+struct\s+(\w+)\s+{/);
    if (parts) {
        context.type = parts[1];
        context.last = 'start';
        toCode('\r\nstruct json_object* build' + S('-' + context.type).camelize().s + '(struct ' + context.type + ' *' + context.type + ') {\r\n');
        context.indent = '    ';
    }

    // match type value
    parts = line.match(/\s+(\w+)\s+(\w+);/);
    if (parts) {
        let key = parts[1];
        let value = parts[2];
        if ('start' === context.last) {
            if ('uint16_t' === key && 'cnt' === value) {
                toCode(context.indent + 'struct json_object *array = json_object_new_array();\r\n');
                toCode(context.indent + 'struct ' + context.type + ' *e = ' + context.type + ';\r\n');
                toCode(context.indent + 'while (e) {\r\n');
                context.indent = '        ';
                toCode(context.indent + 'json_object_object_add(obj, "' + value + '", ' + getNewMethod(key) + '(' + context.type + '->' + value + '));\r\n');
            } else {
                toCode(context.indent + 'struct json_object *obj = json_object_new_object();\r\n');
                toCode(context.indent + 'json_object_object_add(obj, "' + value + '", ' + getNewMethod(key) + '(' + context.type + '->' + value + '));\r\n');
            }
        } else {
            toCode(context.indent + 'json_object_object_add(obj, "' + value + '", ' + getNewMethod(key) + '(' + context.type + '->' + value + '));\r\n');
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
                toCode(context.indent + 'struct json_object *array = json_object_new_array();\r\n');
                toCode(context.indent + 'struct ' + context.type + ' *e = ' + context.type + ';\r\n');
                toCode(context.indent + 'while (e) {\r\n');
                context.indent = '      ';
                toCode(context.indent + 'json_object_object_add(obj, "' + value + '", ' + getNewMethod(key) + '(' + context.type + '->' + value + '));\r\n');
            } else {
                toCode(context.indent + 'struct json_object *obj = json_object_new_object();\r\n');
                toCode(context.indent + 'json_object_object_add(obj, "' + value + '", ' + getNewMethod(key) + '(' + context.type + '->' + value + '));\r\n');
            }
        } else if ('next' === value) {
            toCode(context.indent + 'json_object_array_add(array, obj);\r\n');
            toCode(context.indent + 'e = e->next;\r\n');
            context.indent = '    ';
            toCode(context.indent + '}\r\n');
        } else {
            toCode(context.indent + 'json_object_object_add(obj, "' + value + '", ' + getNewMethod(key) + '(' + context.type + '->' + value + '));\r\n');
        }

        context.last = ('next' === value) ? 'next' : 'pointer';
    }

    // match bitset field => type name : number
    parts = line.match(/.*?(\w+)\s+:\s+\d+[,;]/);
    if (parts) {
        let value = parts[1];
        toCode(context.indent + 'json_object_object_add(obj, "' + value + '", json_object_new_int(' + context.type + '->' + value + '));\r\n');
        context.last = 'bitset';
    }

    // match endline => };
    parts = line.match(/}.*?(\w+);/);
    if (parts) {
        if ('next' === context.last) {
            toCode(context.indent + 'return array;\r\n');
        } else {
            toCode(context.indent + 'return obj;\r\n');
        }
        context.indent = '';
        toCode('}\r\n');
        context.last = 'end';
    }
};

fs.readFile('./head.h', 'utf8', (err, data) => {
    if (err) throw err;
    let lines = data.match(/[^\r\n]+/g);

    let context = {};
    lines.forEach((line, index) => {
        parseLine(line, context);
    });
});
