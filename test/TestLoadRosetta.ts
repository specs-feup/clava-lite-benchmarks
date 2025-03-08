import { FunctionJp } from "@specs-feup/clava/api/Joinpoints.js";
import Query from "@specs-feup/lara/api/weaver/Query.js";
import { ROSETTA } from "../src/BenchmarkSuites.js";
import { loadApp } from "../src/LiteBenchmarkLoader.js";

let suite = ROSETTA;
let app = "spam-filter";
loadApp(suite, app);

for (const fun of Query.search(FunctionJp)) {
    console.log(fun.name);
}

// suite = SuiteSelector.ROSETTA;
// for (const app of suite.apps) {
//     LiteBenchmarkLoader.load(suite, app);

//     console.log(`Functions of app ${app} from benchmark suite ${suite.name}:`);
//     for (const fun of Query.search(FunctionJp)) {
//         console.log(fun.name);
//     }
// }