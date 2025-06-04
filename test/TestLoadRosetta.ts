import { FunctionJp } from "@specs-feup/clava/api/Joinpoints.js";
import Query from "@specs-feup/lara/api/weaver/Query.js";
import { ROSETTA } from "../src/BenchmarkSuites.js";
import { copyDirents, loadSuite } from "../src/LiteBenchmarkLoader.js";
import Clava from "@specs-feup/clava/api/clava/Clava.js";

for (const res of loadSuite(ROSETTA)) {
    if (res.success) {
        console.log(`Loaded app: ${res.app}, top function: ${res.topFunction}`);

        for (const fun of Query.search(FunctionJp)) {
            console.log(fun.name);
        }

        Clava.writeCode(`outputs/Rosetta/${res.app}`);
        copyDirents(res.direntsToCopy, `outputs/Rosetta/${res.app}`);
    }
    else {
        console.log(`Failed to load app: ${res.app}`);
    }
}