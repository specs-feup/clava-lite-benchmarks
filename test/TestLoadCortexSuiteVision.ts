import { loadApp, loadSuite } from "../src/LiteBenchmarkLoader.js";
import { CORTEXSUITE_VISION } from "../src/BenchmarkSuites.js";
import { Amalgamator } from "@specs-feup/clava-code-transforms/Amalgamator";

function handleApp(appName: string): void {
    const merger = new Amalgamator();
    const [file, includes] = merger.amalgamate(appName);
    merger.writeAmalgamation(file, `outputs/${appName}`, includes);
}

function loadOne(appName: string): void {
    let suite = CORTEXSUITE_VISION;
    let app = CORTEXSUITE_VISION.apps[appName];
    console.log(`Loading app: ${appName}`);


    loadApp(suite, app);
    handleApp(appName);
}

function loadAll(): void {
    const loader = loadSuite(CORTEXSUITE_VISION);

    for (const res of loader) {
        if (res.success) {
            console.log(`Loaded app: ${res.app}, top function: ${res.topFunction}`);
            handleApp(res.app);
        }
        else {
            console.log(`Failed to load app: ${res.app}`);
        }
    }
}

//loadOne("vision-disparity");
//loadOne("vision-localization");
//loadOne("vision-mser");
//loadOne("vision-multi-ncut");
//loadOne("vision-pca");
//loadOne("vision-sift");
//loadOne("vision-stitch");
//loadOne("vision-svm");
//loadOne("vision-texture-synthesis");
loadOne("vision-tracking");