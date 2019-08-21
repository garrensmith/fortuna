let lib = {};
const mapFuns = new Map();
let results = [];

function emit(key, value) {
    results.push([key, value]);
}

function setLib(source) {
    lib = JSON.parse(source);
}

function addFun(id, source) {
    print("WOO HOO");
    const fixedSource = rewriteAnonFun(source);
    const fun = eval(fixedSource);
    print(fun);
    if (typeof fun === "function") {
        mapFuns.set(id, fun);
    } else {
        throw "Invalid function";
    }
}

const mapFunRunner = (doc, mapFn) => {
    try {
        results = [];
        mapFn(doc);
        return results;
    } catch (ex) {
        return { error: ex.toString() };
    }
};

function mapDoc(doc_str) {
    const doc = JSON.parse(doc_str);
    const mapResults = Array.from(mapFuns, ([id, mapFun]) => {
        const mapResult = { id };

        const result = mapFunRunner(doc, mapFun);
        if (result.error) {
            mapResult.error = result.error;
        } else {
            mapResult.result = result;
        }

        return mapResult;
    });

    return JSON.stringify(mapResults);
}

function clearFuns() {
    mapFuns.clear();
}

if (typeof module !== "undefined") {
    module.exports = {
        mapDoc,
        addFun,
        setLib,
        clearFuns
    };
}
