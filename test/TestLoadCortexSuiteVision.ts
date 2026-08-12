import { copyDirentsRelative, loadApp, LoadResult, loadSuite } from "../src/LiteBenchmarkLoader.js";
import { CORTEXSUITE_VISION, NOSIZES  } from "../src/BenchmarkSuites.js";
import { FileJp } from "@specs-feup/clava/api/Joinpoints.js";
import Query from "@specs-feup/lara/api/weaver/Query.js";
import Clava from "@specs-feup/clava/api/clava/Clava.js";

function handleApp(res: LoadResult<NOSIZES>): void {
    const name = res.appSummary.canonicalName;

    for (const file of Query.search(FileJp)) {
        console.log(`App: ${name}, file: ${file.filename}`);
    }
    Clava.writeCode(`outputs/CortexSuite-Vision/${name}`);
    copyDirentsRelative(res.relativeDirents!, res.appRoot!, `outputs/CortexSuite-Vision/${name}`);
}

function loadOne(appName: string): void {
    let suite = CORTEXSUITE_VISION;
    let app = CORTEXSUITE_VISION.apps[appName];
    console.log(`Loading app: ${appName}`);


    const res = loadApp(suite, app);
    handleApp(res);
}

function loadAll(): void {
    const loader = loadSuite(CORTEXSUITE_VISION);

    for (const res of loader) {
        const appSummary = res.appSummary;
        if (res.success) {
            console.log(`Loaded app: ${appSummary.canonicalName}, top function: ${appSummary.topFunction}`);
            handleApp(res);
        }
        else {
            console.log(`Failed to load app: ${appSummary.canonicalName}`);
        }
    }
}

loadOne("vision-sift");
//loadAll();
