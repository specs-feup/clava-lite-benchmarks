import { loadApp, loadSuite } from "../src/LiteBenchmarkLoader.js";
import { CORTEXSUITE_VISION } from "../src/BenchmarkSuites.js";
import { Amalgamator } from "@specs-feup/clava-code-transforms/Amalgamator";
import { FileJp } from "@specs-feup/clava/api/Joinpoints.js";
import Query from "@specs-feup/lara/api/weaver/Query.js";
import Clava from "@specs-feup/clava/api/clava/Clava.js";

function handleApp(appName: string): void {
    for (const file of Query.search(FileJp)) {
        console.log(`App: ${appName}, file: ${file.filename}`);
    }
    Clava.writeCode(`outputs/${appName}`);
}

function loadOne(appName: string): void {
    let suite = CORTEXSUITE_VISION;
    let app = CORTEXSUITE_VISION.apps[appName];
    console.log(`Loading app: ${appName}`);


    loadApp(suite, app);
    handleApp(app.canonicalName);
}

function loadAll(): void {
    const loader = loadSuite(CORTEXSUITE_VISION);

    for (const res of loader) {
        if (res.success) {
            console.log(`Loaded app: ${res.app}, top function: ${res.topFunction}`);
            handleApp(res.app.replace("vision-", ""));
        }
        else {
            console.log(`Failed to load app: ${res.app}`);
        }
    }
}

loadOne("vision-svm");
//loadAll();