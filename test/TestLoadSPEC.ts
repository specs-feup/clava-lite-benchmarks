import { FileJp } from "@specs-feup/clava/api/Joinpoints.js";
import Query from "@specs-feup/lara/api/weaver/Query.js";
import { SPEC2017 } from "../src/BenchmarkSuites.js";
import { copyDirents, loadApp, loadSuite } from "../src/LiteBenchmarkLoader.js";
import Clava from "@specs-feup/clava/api/clava/Clava.js";

function handleApp(appName: string, direntsToCopy: string[]): void {
    // Print the name of all files currently on the AST
    for (const file of Query.search(FileJp)) {
        console.log(`App: ${appName}, file: ${file.filename}`);
    }

    // Output the current AST
    Clava.writeCode(`outputs/SPEC2017/${appName}`);

    // Copy any additional files (e.g., input files)
    copyDirents(direntsToCopy, `outputs/SPEC2017/${appName}`);
}

function loadOne(appName: string): void {
    let suite = SPEC2017;
    let app = SPEC2017.apps[appName];
    console.log(`Loading app: ${appName}`);

    const res = loadApp(suite, app);
    handleApp(app.canonicalName, res.direntsToCopy ? res.direntsToCopy : []);
}

function loadAll(): void {
    const loader = loadSuite(SPEC2017);

    for (const res of loader) {
        if (res.success) {
            console.log(`Loaded app: ${res.app}, top function: ${res.topFunction}`);
            handleApp(res.app, res.direntsToCopy ? res.direntsToCopy : []);
        }
        else {
            console.log(`Failed to load app: ${res.app}`);
        }
    }
}

//loadOne("519.lbm_r");
loadAll();