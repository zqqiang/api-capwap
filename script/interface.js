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
            {
                return 'json_object_new_int';
            }
        default:
            return 'Unsupport Type';
    }
};

function parseLine(line, context) {
    let parts = line.match(/typedef\s+struct\s+(\w+)\s+{/);
    if (parts) {
        console.log(parts[1]);
        context.type = parts[1];
        toCode('\r\n\r\nvoid build' + S('-' + context.type).camelize().s + '(struct json_object *params, struct ' + context.type + ' *' + context.type + ') {\r\n');
        toCode('    struct json_object *obj = json_object_new_object();\r\n');
    }

    parts = line.match(/\s+(\w+)\s+(\w+);/);
    if (parts) {
        console.log(parts[1]);
        console.log(parts[2]);
        let key = parts[1];
        let value = parts[2];
        toCode('    json_object_object_add(obj, "' + value + '", ' + getNewMethod(key) + '(' + context.type + '->' + value + '));\r\n');
    }

    parts = line.match(/}\s+(\w+);/);
    if (parts) {
        console.log(parts[1]);
        toCode('    json_object_object_add(params, "' + context.type + '", obj);\r\n');
        toCode('}\r\n');
    }
};

fs.readFile('./head.h', 'utf8', (err, data) => {
    if (err) throw err;
    console.log(data);
    let lines = data.match(/[^\r\n]+/g);

    let context = {};
    lines.forEach((line, index) => {
        parseLine(line, context);
    });
});
