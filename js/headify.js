#!/usr/bin/env node

// Licensed under the Apache License, Version 2.0 (the "License"); you may not
// use this file except in compliance with the License. You may obtain a copy of
// the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations under
// the License.

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