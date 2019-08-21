#!/usr/bin/env node

const fs = require('fs');
const path = require('path');

const [,, ...args] = process.argv;
const fileName = args[0];
const outputFile = args[1];
console.log("headify", fileName, outputFile)

const file = fs.readFileSync(fileName);

const baseFileName = path.basename(fileName, '.js');
console.log('SS', baseFileName);

const output = `
    const char* ${baseFileName}_source = R"(
        ${file.toString()}
    )";
`;

fs.writeFileSync(outputFile, output, {flag: 'w'});