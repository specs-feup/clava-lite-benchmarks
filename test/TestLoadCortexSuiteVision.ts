import { FunctionJp } from "@specs-feup/clava/api/Joinpoints.js";
import Query from "@specs-feup/lara/api/weaver/Query.js";
import { loadApp, loadSuite } from "../src/LiteBenchmarkLoader.js";
import { CORTEXSUITE_VISION } from "../src/BenchmarkSuites.js";

function handleApp(): void {
    for (const fun of Query.search(FunctionJp)) {
        console.log(fun.name);
    }
}

function loadOne(appName: string): void {
    let suite = CORTEXSUITE_VISION;
    let app = CORTEXSUITE_VISION.apps["vision-disparity"];
    console.log(`Loading app: ${appName}`);


    loadApp(suite, app);
    handleApp();
}

function loadAll(): void {
    const loader = loadSuite(CORTEXSUITE_VISION);

    for (const res of loader) {
        if (res.success) {
            console.log(`Loaded app: ${res.app}, top function: ${res.topFunction}`);
            handleApp();
        }
        else {
            console.log(`Failed to load app: ${res.app}`);
        }
    }
}

loadOne("vision-disparity");