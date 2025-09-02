import { FunctionJp } from "@specs-feup/clava/api/Joinpoints.js";
import Query from "@specs-feup/lara/api/weaver/Query.js";
import { POLYBENCH_4_2, POLYBENCH_SIZES } from "../src/BenchmarkSuites.js";
import { copyDirentsAbsolute, loadSuite } from "../src/LiteBenchmarkLoader.js";
import Clava from "@specs-feup/clava/api/clava/Clava.js";


for (const res of loadSuite(POLYBENCH_4_2, undefined, POLYBENCH_SIZES.MEDIUM, POLYBENCH_SIZES.LARGE)) {
    const name = res.appSummary.canonicalName;

    if (res.success) {
        console.log(`Loaded app: ${name}, top function: ${res.appSummary.topFunction}`);

        for (const fun of Query.search(FunctionJp)) {
            console.log(fun.name);
        }

        Clava.writeCode(`dist/Polybench4_2/${name}`);
        copyDirentsAbsolute(res.absoluteDirents!, `dist/Polybench4_2/${name}`);
    }
    else {
        console.log(`Failed to load app: ${name}`);
    }
}