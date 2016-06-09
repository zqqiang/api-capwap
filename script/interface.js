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
    let parts = line.match(/typedef\s+struct\s+(\w+)\s+{/);
    if (parts) {
        context.type = parts[1];
        toCode('\r\nstruct json_object* build' + S('-' + context.type).camelize().s + '(struct ' + context.type + ' *' + context.type + ') {\r\n');
        toCode('    struct json_object *obj = json_object_new_object();\r\n');
    }

    parts = line.match(/\s+(\w+)\s+(\w+);/);
    if (parts) {
        let key = parts[1];
        let value = parts[2];
        toCode('    json_object_object_add(obj, "' + value + '", ' + getNewMethod(key) + '(' + context.type + '->' + value + '));\r\n');
    }

    parts = line.match(/\s+(\w+)\s+\*(\w+);/);
    if (parts) {
        let key = parts[1];
        let value = parts[2];
        toCode('    json_object_object_add(obj, "' + value + '", ' + getNewMethod(key) + '(' + context.type + '->' + value + '));\r\n');
    }

    parts = line.match(/.*?(\w+)\s+:\s+\d+[,;]/);
    if (parts) {
        let value = parts[1];
        toCode('    json_object_object_add(obj, "' + value + '", json_object_new_int(' + context.type + '->' + value + '));\r\n');
    }

    parts = line.match(/}.*?(\w+);/);
    if (parts) {
        toCode('    return obj;\r\n');
        toCode('}\r\n');
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
