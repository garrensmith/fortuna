cmake_minimum_required(VERSION 3.15)

file(COPY . 
    DESTINATION ${PROJECT_BINARY_DIR}/js
    FILES_MATCHING PATTERN "*.js"
    )