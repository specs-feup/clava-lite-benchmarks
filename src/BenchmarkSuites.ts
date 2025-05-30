import { BenchmarkSuite } from "./LiteBenchmarkLoader.js";

export const APPS: BenchmarkSuite = {
    name: "Apps",
    path: "apps/",
    apps: {
        "disparity": { standard: "c11", topFunction: "top_level", canonicalName: "disparity" },
        "doom": { standard: "c11", topFunction: "main", canonicalName: "doom" },
        "edgedetect": { standard: "c++11", topFunction: "edge_detect", canonicalName: "edgedetect" },
        "llama2": { standard: "c11", topFunction: "generate", canonicalName: "llama2" },
        "llama2-transformed": { standard: "c11", topFunction: "llama2_loop", canonicalName: "llama2-transformed" }
    },
    flags: ["-lm"],
};

export const AXBENCH: BenchmarkSuite = {
    name: "AxBench",
    path: "benchmarks/AxBench/",
    apps: {
        "blackscholes": { standard: "c++17", topFunction: "BlkSchlsEqEuroNoDiv", canonicalName: "blackscholes" },
        "fft": { standard: "c++17", topFunction: "radix2DitCooleyTykeyFft", canonicalName: "fft" },
        "inversek2j": { standard: "c++17", topFunction: "main", canonicalName: "inversek2j" },
        "jmeint": { standard: "c++17", topFunction: "main", canonicalName: "jmeint" },
        "jpeg": { standard: "c++17", topFunction: "main", canonicalName: "jpeg" },
        "kmeans": { standard: "c++17", topFunction: "segmentImage", canonicalName: "kmeans" },
        "sobel": { standard: "c++17", topFunction: "main", canonicalName: "sobel" }
    },
    flags: []
};

export const CHSTONE: BenchmarkSuite = {
    name: "CHStone",
    path: "benchmarks/CHStone/",
    apps: {
        "adpcm": { standard: "c11", topFunction: "adpcm_main", canonicalName: "adpcm" },
        "aes": { standard: "c11", topFunction: "aes_main", canonicalName: "aes" },
        "blowfish": { standard: "c11", topFunction: "blowfish_main", canonicalName: "blowfish" },
        "dfadd": { standard: "c11", topFunction: "float64_add", canonicalName: "dfadd" },
        "dfdiv": { standard: "c11", topFunction: "float64_div", canonicalName: "dfdiv" },
        "dfmul": { standard: "c11", topFunction: "float64_mul", canonicalName: "dfmul" },
        "dfsin": { standard: "c11", topFunction: "_sin", canonicalName: "dfsin" },
        "gsm": { standard: "c11", topFunction: "Gsm_LPC_Analysis", canonicalName: "gsm" },
        "jpeg": { standard: "c11", topFunction: "jpeg2bmp_main", canonicalName: "jpeg" },
        "mips": { standard: "c11", topFunction: "mips", canonicalName: "mips" },
        "motion": { standard: "c11", topFunction: "main", canonicalName: "motion" },
        "sha": { standard: "c11", topFunction: "sha_stream", canonicalName: "sha" }
    },
    flags: []
};

export const CORTEXSUITE_CORTEX: BenchmarkSuite = {
    name: "CortexSuite-Cortex",
    path: "benchmarks/CortexSuite-Cortex/",
    apps: {
        "cortex-clustering-kmeans": { standard: "c11", topFunction: "main", canonicalName: "cortex-clustering-kmeans" },
        "cortex-clustering-spectral": { standard: "c11", topFunction: "main", canonicalName: "cortex-clustering-spectral" },
        "cortex-cnn": { standard: "c11", topFunction: "main", canonicalName: "cortex-cnn" },
        "cortex-dnn": { standard: "c11", topFunction: "main", canonicalName: "cortex-dnn" },
        "cortex-lda": { standard: "c11", topFunction: "main", canonicalName: "cortex-lda" },
        "cortex-liblinear": { standard: "c11", topFunction: "main", canonicalName: "cortex-liblinear" },
        "cortex-motion-estimation": { standard: "c11", topFunction: "main", canonicalName: "cortex-motion-estimation" },
        "cortex-rbm": { standard: "c11", topFunction: "main", canonicalName: "cortex-rbm" },
        "cortex-sphinx": { standard: "c11", topFunction: "main", canonicalName: "cortex-sphinx" },
        "cortex-srr": { standard: "c11", topFunction: "main", canonicalName: "cortex-srr" },
        "cortex-svd3": { standard: "c11", topFunction: "main", canonicalName: "cortex-svd3" },
        "cortex-word2vec-compute-accuracy": { standard: "c11", topFunction: "main", canonicalName: "cortex-word2vec-compute-accuracy" },
        "cortex-word2vec-distance": { standard: "c11", topFunction: "main", canonicalName: "cortex-word2vec-distance" },
        "cortex-word2vec-word2phrase": { standard: "c11", topFunction: "main", canonicalName: "cortex-word2vec-word2phrase" },
        "cortex-word2vec-word2vec": { standard: "c11", topFunction: "main", canonicalName: "cortex-word2vec-word2vec" },
        "cortex-word2vec-word-analogy": { standard: "c11", topFunction: "main", canonicalName: "cortex-word2vec-word-analogy" }
    },
    flags: []
};

export const CORTEXSUITE_VISION: BenchmarkSuite = {
    name: "CortexSuite-Vision",
    path: "benchmarks/CortexSuite-Vision/",
    apps: {
        "vision-disparity": {
            standard: "c11",
            topFunction: "getDisparity",
            canonicalName: "disparity",
            amalgamate: true
        },
        "vision-localization": {
            standard: "c11",
            topFunction: "updateState",
            altTopFunction: "initQuartenion",
            canonicalName: "localization",
            amalgamate: true
        },
        "vision-mser": {
            standard: "c11",
            topFunction: "mser",
            canonicalName: "mser",
            amalgamate: true
        },
        "vision-multi-ncut": {
            standard: "c11",
            topFunction: "segment_image",
            canonicalName: "multi-ncut",
            amalgamate: true
        },
        "vision-pca": {
            standard: "c11",
            topFunction: "pca",
            canonicalName: "pca",
            amalgamate: true
        },
        "vision-sift": {
            standard: "c11",
            topFunction: "normalizeAndSIFT",
            canonicalName: "sift",
            amalgamate: true
        },
        "vision-stitch": {
            standard: "c11",
            topFunction: "stitch",
            canonicalName: "stitch",
            amalgamate: true
        },
        "vision-svm": {
            standard: "c11",
            topFunction: "svm",
            canonicalName: "svm",
            amalgamate: true
        },
        "vision-texture-synthesis": {
            standard: "c11",
            topFunction: "create_texture",
            canonicalName: "texture-synthesis",
            amalgamate: true
        },
        "vision-tracking": {
            standard: "c11",
            topFunction: "trackFeaturesPyramidalLK",
            altTopFunction: "imagePreprocessing",
            canonicalName: "tracking",
            amalgamate: true
        }
    },
    flags: []
};

export const MACHSUITE: BenchmarkSuite = {
    name: "MachSuite",
    path: "benchmarks/MachSuite/",
    apps: {
        "aes": { standard: "c11", topFunction: "aes256_encrypt_ecb", canonicalName: "aes" },
        "backprop": { standard: "c11", topFunction: "backprop", canonicalName: "backprop" },
        "bfs-bulk": { standard: "c11", topFunction: "bfs", canonicalName: "bfs-bulk" },
        "bfs-queue": { standard: "c11", topFunction: "bfs", canonicalName: "bfs-queue" },
        "fft-strided": { standard: "c11", topFunction: "fft", canonicalName: "fft-strided" },
        "fft-transpose": { standard: "c11", topFunction: "fft1D_512", canonicalName: "fft-transpose" },
        "gemm-blocked": { standard: "c11", topFunction: "bbgemm", canonicalName: "gemm-blocked" },
        "gemm-ncubed": { standard: "c11", topFunction: "gemm", canonicalName: "gemm-ncubed" },
        "kmp": { standard: "c11", topFunction: "kmp", canonicalName: "kmp" },
        "md-grid": { standard: "c11", topFunction: "md", canonicalName: "md-grid" },
        "md-knn": { standard: "c11", topFunction: "md_kernel", canonicalName: "md-knn" },
        "nw": { standard: "c11", topFunction: "needwun", canonicalName: "nw" },
        "sort-merge": { standard: "c11", topFunction: "ms_mergesort", canonicalName: "sort-merge" },
        "sort-radix": { standard: "c11", topFunction: "ss_sort", canonicalName: "sort-radix" },
        "spmv-crs": { standard: "c11", topFunction: "spmv", canonicalName: "spmv-crs" },
        "spmv-ellpack": { standard: "c11", topFunction: "ellpack", canonicalName: "spmv-ellpack" },
        "stencil-2d": { standard: "c11", topFunction: "stencil", canonicalName: "stencil-2d" },
        "stencil-3d": { standard: "c11", topFunction: "stencil3d", canonicalName: "stencil-3d" },
        "viterbi": { standard: "c11", topFunction: "viterbi", canonicalName: "viterbi" }
    },
    flags: []
};

export const RODINIA: BenchmarkSuite = {
    name: "Rodinia",
    path: "benchmarks/Rodinia/",
    apps: {
        "backprop": { standard: "c11", topFunction: "main", canonicalName: "backprop" },
        "bfs": { standard: "c11", topFunction: "main", canonicalName: "bfs" },
        "b+tree": { standard: "c11", topFunction: "main", canonicalName: "b+tree" },
        "cfd-euler3d": { standard: "c11", topFunction: "main", canonicalName: "cfd-euler3d" },
        "cfd-euler3d-double": { standard: "c11", topFunction: "main", canonicalName: "cfd-euler3d-double" },
        "cfd-pre-euler3d": { standard: "c11", topFunction: "main", canonicalName: "cfd-pre-euler3d" },
        "cfd-pre-euler3d-double": { standard: "c11", topFunction: "main", canonicalName: "cfd-pre-euler3d-double" },
        "heartwall": { standard: "c11", topFunction: "main", canonicalName: "heartwall" },
        "hotspot": { standard: "c11", topFunction: "main", canonicalName: "hotspot" },
        "hotspot3D": { standard: "c11", topFunction: "main", canonicalName: "hotspot3D" },
        "kmeans": { standard: "c11", topFunction: "main", canonicalName: "kmeans" },
        "lavaMD": { standard: "c11", topFunction: "main", canonicalName: "lavaMD" },
        "leukocyte": { standard: "c11", topFunction: "main", canonicalName: "leukocyte" },
        "lud": { standard: "c11", topFunction: "main", canonicalName: "lud" },
        "myocyte": { standard: "c11", topFunction: "main", canonicalName: "myocyte" },
        "nn": { standard: "c11", topFunction: "main", canonicalName: "nn" },
        "nw": { standard: "c11", topFunction: "main", canonicalName: "nw" },
        "particlefilter": { standard: "c11", topFunction: "main", canonicalName: "particlefilter" },
        "pathfinder": { standard: "c11", topFunction: "main", canonicalName: "pathfinder" },
        "srad-v1": { standard: "c11", topFunction: "main", canonicalName: "srad-v1" },
        "srad-v2": { standard: "c11", topFunction: "main", canonicalName: "srad-v2" },
        "streamcluster": { standard: "c11", topFunction: "main", canonicalName: "streamcluster" }
    },
    flags: []
};

export const ROSETTA: BenchmarkSuite = {
    name: "Rosetta",
    path: "benchmarks/Rosetta/",
    apps: {
        "3d-rendering": { standard: "c++11", topFunction: "rendering_sw", canonicalName: "3d-rendering" },
        "digit-recognition": { standard: "c++11", topFunction: "DigitRec_sw", canonicalName: "digit-recognition" },
        "face-detection": { standard: "c++11", topFunction: "face_detect_sw", canonicalName: "face-detection" },
        "optical-flow": { standard: "c++11", topFunction: "optical_flow_sw", canonicalName: "optical-flow" },
        "spam-filter": { standard: "c++11", topFunction: "SgdLR_sw", canonicalName: "spam-filter", altTopFunction: "main" }
    },
    flags: ["-D SW"]
};

export const SPEC2017: BenchmarkSuite = {
    name: "SPEC2017",
    path: "benchmarks/SPEC2017/",
    apps: {
        "519_lbm_r": { standard: "c11", topFunction: "main", canonicalName: "519.lbm_r" },
        "531_deepsjeng_r": { standard: "c++11", topFunction: "main", canonicalName: "531.deepsjeng_r" }
    },
    flags: ["-lm"]
};