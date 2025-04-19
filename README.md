# clava-lite-benchmarks

A simple, reusable benchmark loader for NPM-based Clava packages.

## Usage

```TypeScript
import Query from "@specs-feup/lara/api/weaver/Query.js";
import { FunctionJp } from "@specs-feup/clava/api/Joinpoints.js";
import { LiteBenchmarkLoader } from "clava-lite-benchmarks/LiteBenchmarkLoader";
import { SuiteSelector } from "clava-lite-benchmarks/SuiteSelector";

const suite = SuiteSelector.ROSETTA;
for (const app of suite.apps) {
    LiteBenchmarkLoader.load(suite, app);

    console.log(`Functions of app ${app} from benchmark suite ${suite.name}:`);
    for (const fun of Query.search(FunctionJp)) {
        console.log(fun.name);
    }
}
```

## Licenses

This Clava extension is licensed under LGPL-3.0 License. You can use, modify, and distribute it under the terms of this license.

The included benchmark applications have different licenses, which are provided alongside their code.
