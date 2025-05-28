import { FunctionJp } from "@specs-feup/clava/api/Joinpoints.js";
import Query from "@specs-feup/lara/api/weaver/Query.js";
import { loadSuite } from "../src/LiteBenchmarkLoader.js";
import { CORTEXSUITE_VISION } from "../src/BenchmarkSuites.js";

const loader = loadSuite(CORTEXSUITE_VISION);

for (const res of loader) {
    if (res.success) {
        console.log(`Loaded app: ${res.app}, top function: ${res.topFunction}`);

        for (const fun of Query.search(FunctionJp)) {
            console.log(fun.name);
        }
    }
    else {
        console.log(`Failed to load app: ${res.app}`);
    }
}