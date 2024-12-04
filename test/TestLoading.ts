import { FunctionJp } from "@specs-feup/clava/api/Joinpoints.js";
import { LiteBenchmarkLoader } from "../src/LiteBenchmarkLoader.js";
import { SuiteSelector } from "../src/SuiteSelector.js";
import Query from "@specs-feup/lara/api/weaver/Query.js";

let suite = SuiteSelector.APPS;
let app = "edgedetect";
LiteBenchmarkLoader.load(suite, app);

for (const fun of Query.search(FunctionJp)) {
    console.log(fun.name);
}

suite = SuiteSelector.ROSETTA;
app = "spam-filter";
LiteBenchmarkLoader.load(suite, app);

for (const fun of Query.search(FunctionJp)) {
    console.log(fun.name);
}

suite = SuiteSelector.ROSETTA;
for (const app of suite.apps) {
    LiteBenchmarkLoader.load(suite, app);

    console.log(`Functions of app ${app} from benchmark suite ${suite.name}:`);
    for (const fun of Query.search(FunctionJp)) {
        console.log(fun.name);
    }
}