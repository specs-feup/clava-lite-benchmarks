# clava-lite-benchmarks

A simple, reusable benchmark loader for NPM-based Clava packages.

## Usage

* Example 1: Loading an entire suite, one application at a time:

```TypeScript
import Query from "@specs-feup/lara/api/weaver/Query.js";
import { FunctionJp } from "@specs-feup/clava/api/Joinpoints.js";
import { loadSuite } from "@specs-feup/clava-lite-benchmarks/LiteBenchmarkLoader";
import { ROSETTA } from "@specs-feup/clava-lite-benchmarks/BenchmarkSuites";

const loader = loadSuite(ROSETTA);

for (const res of loader) {
    if (res.success) {
        console.log(`Loaded app: ${res.app}, top function: ${res.topFunction}`);

        // Run whatever script you want over the loaded application
        // For this example, we simply print the name of every function
        for (const fun of Query.search(FunctionJp)) {
            console.log(fun.name);
        }
    }
    else {
        console.log(`Failed to load app: ${res.app}`);
    }
}
```

* Example 2: loading a specific application, provided we know its name

```TypeScript
import Query from "@specs-feup/lara/api/weaver/Query.js";
import { FunctionJp } from "@specs-feup/clava/api/Joinpoints.js";
import { loadApp } from "@specs-feup/clava-lite-benchmarks/LiteBenchmarkLoader";
import { ROSETTA } from "@specs-feup/clava-lite-benchmarks/BenchmarkSuites";

let suite = ROSETTA;
let app = "spam-filter"
loadApp(suite, app);

for (const fun of Query.search(FunctionJp)) {
    console.log(fun.name);
}
```

## Licenses

This Clava extension is licensed under LGPL-3.0 License. You can use, modify, and distribute it under the terms of this license.

The included benchmark applications have different licenses, which are provided alongside their code.
