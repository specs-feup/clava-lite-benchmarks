import { FunctionJp } from "@specs-feup/clava/api/Joinpoints.js";
import Query from "@specs-feup/lara/api/weaver/Query.js";
import { ROSETTA } from "../src/BenchmarkSuites.js";
import { copyDirentsAbsolute, loadSuite } from "../src/LiteBenchmarkLoader.js";
import Clava from "@specs-feup/clava/api/clava/Clava.js";


for (const res of loadSuite(ROSETTA)) {
    const name = res.appSummary.canonicalName;

    if (res.success) {
        console.log(`Loaded app: ${name}, top function: ${res.appSummary.topFunction}`);

        for (const fun of Query.search(FunctionJp)) {
            console.log(fun.name);
        }

        Clava.writeCode(`outputs/Rosetta/${name}`);
        copyDirentsAbsolute(res.absoluteDirents!, `outputs/Rosetta/${name}`);
    }
    else {
        console.log(`Failed to load app: ${name}`);
    }
}