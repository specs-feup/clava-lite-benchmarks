import { BenchmarkSuite } from "./LiteBenchmarkLoader.js";

export enum NOSIZES {
    NA = ""
}

class Apps extends BenchmarkSuite<NOSIZES> {
    public constructor() {
        super(
            "Apps",
            NOSIZES,
            "apps/",
            {
                "disparity": { standard: "c11", topFunction: "top_level", canonicalName: "disparity", invalidInputSizes: [] },
                "doom": { standard: "c11", topFunction: "main", canonicalName: "doom", invalidInputSizes: [] },
                "edgedetect": { standard: "c++11", topFunction: "edge_detect", canonicalName: "edgedetect", invalidInputSizes: [] },
                "llama2": { standard: "c11", topFunction: "generate", canonicalName: "llama2", invalidInputSizes: [] },
                "llama2-transformed": { standard: "c11", topFunction: "llama2_loop", canonicalName: "llama2-transformed", invalidInputSizes: [] }
            },
            ["-lm"]
        )
    }
};

export const APPS: Apps = new Apps();

class AXBench extends BenchmarkSuite<NOSIZES> {
    public constructor() {
        super(
            "AxBench",
            NOSIZES,
            "benchmarks/AxBench/",
            {
                "blackscholes": { standard: "c++17", topFunction: "BlkSchlsEqEuroNoDiv", canonicalName: "blackscholes", invalidInputSizes: [] },
                "fft": { standard: "c++17", topFunction: "radix2DitCooleyTykeyFft", canonicalName: "fft", invalidInputSizes: [] },
                "inversek2j": { standard: "c++17", topFunction: "main", canonicalName: "inversek2j", invalidInputSizes: [] },
                "jmeint": { standard: "c++17", topFunction: "main", canonicalName: "jmeint", invalidInputSizes: [] },
                "jpeg": { standard: "c++17", topFunction: "main", canonicalName: "jpeg", invalidInputSizes: [] },
                "kmeans": { standard: "c++17", topFunction: "segmentImage", canonicalName: "kmeans", invalidInputSizes: [] },
                "sobel": { standard: "c++17", topFunction: "main", canonicalName: "sobel", invalidInputSizes: [] }
            },
            []
        )
    }
};

export const AXBENCH: AXBench = new AXBench();

class CHStone extends BenchmarkSuite<NOSIZES> {
    public constructor() {
        super(
            "CHStone",
            NOSIZES,
            "benchmarks/CHStone/",
            {
                "adpcm": { standard: "c11", topFunction: "adpcm_main", canonicalName: "adpcm", invalidInputSizes: [] },
                "aes": { standard: "c11", topFunction: "aes_main", canonicalName: "aes", invalidInputSizes: [] },
                "blowfish": { standard: "c11", topFunction: "blowfish_main", canonicalName: "blowfish", invalidInputSizes: [] },
                "dfadd": { standard: "c11", topFunction: "float64_add", canonicalName: "dfadd", invalidInputSizes: [] },
                "dfdiv": { standard: "c11", topFunction: "float64_div", canonicalName: "dfdiv", invalidInputSizes: [] },
                "dfmul": { standard: "c11", topFunction: "float64_mul", canonicalName: "dfmul", invalidInputSizes: [] },
                "dfsin": { standard: "c11", topFunction: "_sin", canonicalName: "dfsin", invalidInputSizes: [] },
                "gsm": { standard: "c11", topFunction: "Gsm_LPC_Analysis", canonicalName: "gsm", invalidInputSizes: [] },
                "jpeg": { standard: "c11", topFunction: "jpeg2bmp_main", canonicalName: "jpeg", invalidInputSizes: [] },
                "mips": { standard: "c11", topFunction: "mips", canonicalName: "mips", invalidInputSizes: [] },
                "motion": { standard: "c11", topFunction: "main", canonicalName: "motion", invalidInputSizes: [] },
                "sha": { standard: "c11", topFunction: "sha_stream", canonicalName: "sha", invalidInputSizes: [] }
            },
            []
        );
    }
};

export const CHSTONE: CHStone = new CHStone();

class CortexSuite_Cortex extends BenchmarkSuite<NOSIZES> {
    public constructor() {
        super(
            "CortexSuite-Cortex",
            NOSIZES,
            "benchmarks/CortexSuite-Cortex/",
            {
                "cortex-clustering-kmeans": { standard: "c11", topFunction: "main", canonicalName: "cortex-clustering-kmeans", invalidInputSizes: [] },
                "cortex-clustering-spectral": { standard: "c11", topFunction: "main", canonicalName: "cortex-clustering-spectral", invalidInputSizes: [] },
                "cortex-cnn": { standard: "c11", topFunction: "main", canonicalName: "cortex-cnn", invalidInputSizes: [] },
                "cortex-dnn": { standard: "c11", topFunction: "main", canonicalName: "cortex-dnn", invalidInputSizes: [] },
                "cortex-lda": { standard: "c11", topFunction: "main", canonicalName: "cortex-lda", invalidInputSizes: [] },
                "cortex-liblinear": { standard: "c11", topFunction: "main", canonicalName: "cortex-liblinear", invalidInputSizes: [] },
                "cortex-motion-estimation": { standard: "c11", topFunction: "main", canonicalName: "cortex-motion-estimation", invalidInputSizes: [] },
                "cortex-rbm": { standard: "c11", topFunction: "main", canonicalName: "cortex-rbm", invalidInputSizes: [] },
                "cortex-sphinx": { standard: "c11", topFunction: "main", canonicalName: "cortex-sphinx", invalidInputSizes: [] },
                "cortex-srr": { standard: "c11", topFunction: "main", canonicalName: "cortex-srr", invalidInputSizes: [] },
                "cortex-svd3": { standard: "c11", topFunction: "main", canonicalName: "cortex-svd3", invalidInputSizes: [] },
                "cortex-word2vec-compute-accuracy": { standard: "c11", topFunction: "main", canonicalName: "cortex-word2vec-compute-accuracy", invalidInputSizes: [] },
                "cortex-word2vec-distance": { standard: "c11", topFunction: "main", canonicalName: "cortex-word2vec-distance", invalidInputSizes: [] },
                "cortex-word2vec-word2phrase": { standard: "c11", topFunction: "main", canonicalName: "cortex-word2vec-word2phrase", invalidInputSizes: [] },
                "cortex-word2vec-word2vec": { standard: "c11", topFunction: "main", canonicalName: "cortex-word2vec-word2vec", invalidInputSizes: [] },
                "cortex-word2vec-word-analogy": { standard: "c11", topFunction: "main", canonicalName: "cortex-word2vec-word-analogy", invalidInputSizes: [] }
            },
            []
        );
    }

};

export const CORTEXSUITE_CORTEX: CortexSuite_Cortex = new CortexSuite_Cortex();

class CortexSuite_Vision extends BenchmarkSuite<NOSIZES> {
    public constructor() {
        super(
            "CortexSuite-Vision",
            NOSIZES,
            "benchmarks/CortexSuite-Vision/",
            {
                "vision-disparity": {
                    standard: "c11",
                    topFunction: "getDisparity",
                    canonicalName: "disparity",
                    amalgamate: true,
                    extraFlags: ["-Dfullhd"],
                    extraFiles: ["../Makefile", "data/fullhd"],
                    invalidInputSizes: []
                },
                "vision-localization": {
                    standard: "c11",
                    topFunction: "updateState",
                    altTopFunction: "initQuartenion",
                    canonicalName: "localization",
                    amalgamate: true,
                    extraFlags: ["-Dvga"],
                    extraFiles: ["../Makefile", "data/vga"],
                    invalidInputSizes: []
                },
                "vision-mser": {
                    standard: "c11",
                    topFunction: "mser",
                    canonicalName: "mser",
                    amalgamate: true,
                    extraFlags: ["-Dfullhd"],
                    extraFiles: ["../Makefile", "data/fullhd"],
                    invalidInputSizes: []
                },
                "vision-multi-ncut": {
                    standard: "c11",
                    topFunction: "segment_image",
                    canonicalName: "multi-ncut",
                    amalgamate: true,
                    extraFlags: ["-Dqcif"],
                    extraFiles: ["../Makefile", "data/qcif"],
                    invalidInputSizes: []
                },
                "vision-pca": {
                    standard: "c11",
                    topFunction: "pca",
                    canonicalName: "pca",
                    amalgamate: true,
                    extraFiles: ["../Makefile", "data"],
                    invalidInputSizes: []
                },
                "vision-sift": {
                    standard: "c11",
                    topFunction: "normalizeAndSIFT",
                    canonicalName: "sift",
                    amalgamate: true,
                    extraFlags: ["-Dfullhd"],
                    extraFiles: ["../Makefile", "data/fullhd"],
                    invalidInputSizes: []
                },
                "vision-stitch": {
                    standard: "c11",
                    topFunction: "stitch",
                    canonicalName: "stitch",
                    amalgamate: true,
                    extraFlags: ["-Dfullhd"],
                    extraFiles: ["../Makefile", "data/fullhd"],
                    invalidInputSizes: []
                },
                "vision-svm": {
                    standard: "c11",
                    topFunction: "svm",
                    canonicalName: "svm",
                    amalgamate: true,
                    extraFlags: ["-Dcif"],
                    extraFiles: ["../Makefile", "data/cif"],
                    invalidInputSizes: []
                },
                "vision-texture-synthesis": {
                    standard: "c11",
                    topFunction: "create_texture",
                    canonicalName: "texture-synthesis",
                    amalgamate: true,
                    extraFlags: ["-Dfullhd"],
                    extraFiles: ["../Makefile", "data/fullhd"],
                    invalidInputSizes: []
                },
                "vision-tracking": {
                    standard: "c11",
                    topFunction: "trackFeaturesPyramidalLK",
                    altTopFunction: "imagePreprocessing",
                    canonicalName: "tracking",
                    amalgamate: true,
                    extraFlags: ["-Dfullhd"],
                    extraFiles: ["../Makefile", "data/fullhd"],
                    invalidInputSizes: []
                }
            },
            ["-DGENERATE_OUTPUT", "-DCHECK", "-lm"]
        );
    }

};

export const CORTEXSUITE_VISION: CortexSuite_Vision = new CortexSuite_Vision();

class MachSuite extends BenchmarkSuite<NOSIZES> {
    public constructor() {
        super(
            "MachSuite",
            NOSIZES,
            "benchmarks/MachSuite/",
            {
                "aes": { standard: "c11", topFunction: "aes256_encrypt_ecb", canonicalName: "aes", invalidInputSizes: [] },
                "backprop": { standard: "c11", topFunction: "backprop", canonicalName: "backprop", invalidInputSizes: [] },
                "bfs-bulk": { standard: "c11", topFunction: "bfs", canonicalName: "bfs-bulk", invalidInputSizes: [] },
                "bfs-queue": { standard: "c11", topFunction: "bfs", canonicalName: "bfs-queue", invalidInputSizes: [] },
                "fft-strided": { standard: "c11", topFunction: "fft", canonicalName: "fft-strided", invalidInputSizes: [] },
                "fft-transpose": { standard: "c11", topFunction: "fft1D_512", canonicalName: "fft-transpose", invalidInputSizes: [] },
                "gemm-blocked": { standard: "c11", topFunction: "bbgemm", canonicalName: "gemm-blocked", invalidInputSizes: [] },
                "gemm-ncubed": { standard: "c11", topFunction: "gemm", canonicalName: "gemm-ncubed", invalidInputSizes: [] },
                "kmp": { standard: "c11", topFunction: "kmp", canonicalName: "kmp", invalidInputSizes: [] },
                "md-grid": { standard: "c11", topFunction: "md", canonicalName: "md-grid", invalidInputSizes: [] },
                "md-knn": { standard: "c11", topFunction: "md_kernel", canonicalName: "md-knn", invalidInputSizes: [] },
                "nw": { standard: "c11", topFunction: "needwun", canonicalName: "nw", invalidInputSizes: [] },
                "sort-merge": { standard: "c11", topFunction: "ms_mergesort", canonicalName: "sort-merge", invalidInputSizes: [] },
                "sort-radix": { standard: "c11", topFunction: "ss_sort", canonicalName: "sort-radix", invalidInputSizes: [] },
                "spmv-crs": { standard: "c11", topFunction: "spmv", canonicalName: "spmv-crs", invalidInputSizes: [] },
                "spmv-ellpack": { standard: "c11", topFunction: "ellpack", canonicalName: "spmv-ellpack", invalidInputSizes: [] },
                "stencil-2d": { standard: "c11", topFunction: "stencil", canonicalName: "stencil-2d", invalidInputSizes: [] },
                "stencil-3d": { standard: "c11", topFunction: "stencil3d", canonicalName: "stencil-3d", invalidInputSizes: [] },
                "viterbi": { standard: "c11", topFunction: "viterbi", canonicalName: "viterbi", invalidInputSizes: [] }
            },
            []
        );
    }
};

export const MACHSUITE: MachSuite = new MachSuite();

class Rodinia extends BenchmarkSuite<NOSIZES> {
    public constructor() {
        super(
            "Rodinia",
            NOSIZES,
            "benchmarks/Rodinia/",
            {
                "backprop": { standard: "c11", topFunction: "main", canonicalName: "backprop", invalidInputSizes: [] },
                "bfs": { standard: "c11", topFunction: "main", canonicalName: "bfs", invalidInputSizes: [] },
                "b+tree": { standard: "c11", topFunction: "main", canonicalName: "b+tree", invalidInputSizes: [] },
                "cfd-euler3d": { standard: "c11", topFunction: "main", canonicalName: "cfd-euler3d", invalidInputSizes: [] },
                "cfd-euler3d-double": { standard: "c11", topFunction: "main", canonicalName: "cfd-euler3d-double", invalidInputSizes: [] },
                "cfd-pre-euler3d": { standard: "c11", topFunction: "main", canonicalName: "cfd-pre-euler3d", invalidInputSizes: [] },
                "cfd-pre-euler3d-double": { standard: "c11", topFunction: "main", canonicalName: "cfd-pre-euler3d-double", invalidInputSizes: [] },
                "heartwall": { standard: "c11", topFunction: "main", canonicalName: "heartwall", invalidInputSizes: [] },
                "hotspot": { standard: "c11", topFunction: "main", canonicalName: "hotspot", invalidInputSizes: [] },
                "hotspot3D": { standard: "c11", topFunction: "main", canonicalName: "hotspot3D", invalidInputSizes: [] },
                "kmeans": { standard: "c11", topFunction: "main", canonicalName: "kmeans", invalidInputSizes: [] },
                "lavaMD": { standard: "c11", topFunction: "main", canonicalName: "lavaMD", invalidInputSizes: [] },
                "leukocyte": { standard: "c11", topFunction: "main", canonicalName: "leukocyte", invalidInputSizes: [] },
                "lud": { standard: "c11", topFunction: "main", canonicalName: "lud", invalidInputSizes: [] },
                "myocyte": { standard: "c11", topFunction: "main", canonicalName: "myocyte", invalidInputSizes: [] },
                "nn": { standard: "c11", topFunction: "main", canonicalName: "nn", invalidInputSizes: [] },
                "nw": { standard: "c11", topFunction: "main", canonicalName: "nw", invalidInputSizes: [] },
                "particlefilter": { standard: "c11", topFunction: "main", canonicalName: "particlefilter", invalidInputSizes: [] },
                "pathfinder": { standard: "c11", topFunction: "main", canonicalName: "pathfinder", invalidInputSizes: [] },
                "srad-v1": { standard: "c11", topFunction: "main", canonicalName: "srad-v1", invalidInputSizes: [] },
                "srad-v2": { standard: "c11", topFunction: "main", canonicalName: "srad-v2", invalidInputSizes: [] },
                "streamcluster": { standard: "c11", topFunction: "main", canonicalName: "streamcluster", invalidInputSizes: [] }
            },
            []
        );
    }
};

export const RODINIA: Rodinia = new Rodinia();

class Rosetta extends BenchmarkSuite<NOSIZES> {
    public constructor() {
        super(
            "Rosetta",
            NOSIZES,
            "benchmarks/Rosetta/",
            {
                "3d-rendering": {
                    standard: "c++11",
                    topFunction: "rendering_sw",
                    canonicalName: "3d-rendering",
                    amalgamate: true,
                    invalidInputSizes: []
                },
                "digit-recognition": {
                    standard: "c++11",
                    topFunction: "DigitRec_sw",
                    canonicalName: "digit-recognition",
                    amalgamate: true,
                    invalidInputSizes: []
                },
                "face-detection": {
                    standard: "c++11",
                    topFunction: "face_detect_sw",
                    canonicalName: "face-detection",
                    amalgamate: true,
                    invalidInputSizes: []
                },
                "optical-flow": {
                    standard: "c++11",
                    topFunction: "optical_flow_sw",
                    canonicalName: "optical-flow",
                    amalgamate: true,
                    invalidInputSizes: []
                },
                "spam-filter": {
                    standard: "c++11",
                    topFunction: "SgdLR_sw",
                    canonicalName: "spam-filter",
                    altTopFunction: "main",
                    amalgamate: true,
                    invalidInputSizes: []
                }
            },
            ["-D SW"]
        );
    }
};

export const ROSETTA: Rosetta = new Rosetta();

class Spec2017 extends BenchmarkSuite<NOSIZES> {
    public constructor() {
        super(
            "SPEC2017",
            NOSIZES,
            "benchmarks/SPEC2017/",
            {
                "519_lbm_r": { standard: "c11", topFunction: "main", canonicalName: "519.lbm_r", invalidInputSizes: [] },
                "531_deepsjeng_r": { standard: "c++11", topFunction: "main", canonicalName: "531.deepsjeng_r", invalidInputSizes: [] }
            },
            ["-lm"]
        );
    }
};

export const SPEC2017: Spec2017 = new Spec2017();

export enum POLYBENCH_SIZES {
    MINI = "-DMINI_DATASET",
    SMALL = "-DSMALL_DATASET",
    MEDIUM = "-DMEDIUM_DATASET",
    LARGE = "-DLARGE_DATASET",
    EXTRALARGE = "-DEXTRALARGE_DATASET"
}

class PolyBench_4_2 extends BenchmarkSuite<POLYBENCH_SIZES> {
    public constructor() {
        super(
            "POLYBENCH4.2",
            POLYBENCH_SIZES,
            "benchmarks/Polybench/4.2/",
            {
                "2mm": {
                    standard: "gnu11", topFunction: "main", canonicalName: "2mm", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "3mm": {
                    standard: "gnu11", topFunction: "main", canonicalName: "3mm", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "adi": {
                    standard: "gnu11", topFunction: "main", canonicalName: "adi", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "atax": {
                    standard: "gnu11", topFunction: "main", canonicalName: "atax", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "bicg": {
                    standard: "gnu11", topFunction: "main", canonicalName: "bicg", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "cholesky": {
                    standard: "gnu11", topFunction: "main", canonicalName: "cholesky", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "correlation": {
                    standard: "gnu11", topFunction: "main", canonicalName: "correlation", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "covariance": {
                    standard: "gnu11", topFunction: "main", canonicalName: "covariance", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "deriche": {
                    standard: "gnu11", topFunction: "main", canonicalName: "deriche", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "doitgen": {
                    standard: "gnu11", topFunction: "main", canonicalName: "doitgen", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "durbin": {
                    standard: "gnu11", topFunction: "main", canonicalName: "durbin", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "gemm": {
                    standard: "gnu11", topFunction: "main", canonicalName: "gemm", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "gemver": {
                    standard: "gnu11", topFunction: "main", canonicalName: "gemver", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "gesummv": {
                    standard: "gnu11", topFunction: "main", canonicalName: "gesummv", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "gramschmidt": {
                    standard: "gnu11", topFunction: "main", canonicalName: "gramschmidt", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "heat-3d": {
                    standard: "gnu11", topFunction: "main", canonicalName: "heat-3d", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "jacobi-1d": {
                    standard: "gnu11", topFunction: "main", canonicalName: "jacobi-1d", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "jacobi-2d": {
                    standard: "gnu11", topFunction: "main", canonicalName: "jacobi-2d", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "lu": {
                    standard: "gnu11", topFunction: "main", canonicalName: "lu", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "ludcmp": {
                    standard: "gnu11", topFunction: "main", canonicalName: "ludcmp", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "mvt": {
                    standard: "gnu11", topFunction: "main", canonicalName: "mvt", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "seidel-2d": {
                    standard: "gnu11", topFunction: "main", canonicalName: "seidel-2d", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "symm": {
                    standard: "gnu11", topFunction: "main", canonicalName: "symm", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "syr2k": {
                    standard: "gnu11", topFunction: "main", canonicalName: "syr2k", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "syrk": {
                    standard: "gnu11", topFunction: "main", canonicalName: "syrk", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "trisolv": {
                    standard: "gnu11", topFunction: "main", canonicalName: "trisolv", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                },
                "trmm": {
                    standard: "gnu11", topFunction: "main", canonicalName: "trmm", extraSourceFiles: ["../polybench"], invalidInputSizes: []
                }
            },
            ["-lm", "-DPOLYBENCH_TIME", "-DDATA_TYPE_IS_CHAR"]
        );
    }
}

export const POLYBENCH_4_2: PolyBench_4_2 = new PolyBench_4_2();