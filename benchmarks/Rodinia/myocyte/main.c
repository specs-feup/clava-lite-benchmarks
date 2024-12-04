//====================================================================================================100
//		UPDATE
//====================================================================================================100

// Lukasz G. Szafaryn 24 JAN 09

//====================================================================================================100
//		DESCRIPTION
//====================================================================================================100

// Myocyte application models cardiac myocyte (heart muscle cell) and simulates its behavior according to the work by Saucerman and Bers [8]. The model integrates
// cardiac myocyte electrical activity with the calcineurin pathway, which is a key aspect of the development of heart failure. The model spans large number of temporal
// scales to reflect how changes in heart rate as observed during exercise or stress contribute to calcineurin pathway activation, which ultimately leads to the expression
// of numerous genes that remodel the heart�s structure. It can be used to identify potential therapeutic targets that may be useful for the treatment of heart failure.
// Biochemical reactions, ion transport and electrical activity in the cell are modeled with 91 ordinary differential equations (ODEs) that are determined by more than 200
// experimentally validated parameters. The model is simulated by solving this group of ODEs for a specified time interval. The process of ODE solving is based on the
// causal relationship between values of ODEs at different time steps, thus it is mostly sequential. At every dynamically determined time step, the solver evaluates the
// model consisting of a set of 91 ODEs and 480 supporting equations to determine behavior of the system at that particular time instance. If evaluation results are not
// within the expected tolerance at a given time step (usually as a result of incorrect determination of the time step), another calculation attempt is made at a modified
// (usually reduced) time step. Since the ODEs are stiff (exhibit fast rate of change within short time intervals), they need to be simulated at small time scales with an
// adaptive step size solver.

//	1) The original version of the current solver code was obtained from: Mathematics Source Library (http://mymathlib.webtrellis.net/index.html). The solver has been
//      somewhat modified to tailor it to our needs. However, it can be reverted back to original form or modified to suit other simulations.
// 2) This solver and particular solving algorithm used with it (embedded_fehlberg_7_8) were adapted to work with a set of equations, not just one like in original version.
//	3) In order for solver to provide deterministic number of steps (needed for particular amount of memore previousely allocated for results), every next step is
//      incremented by 1 time unit (h_init).
//	4) Function assumes that simulation starts at some point of time (whatever time the initial values are provided for) and runs for the number of miliseconds (xmax)
//      specified by the uses as a parameter on command line.
// 5) The appropriate amount of memory is previousely allocated for that range (y).
//	6) This setup in 3) - 5) allows solver to adjust the step ony from current time instance to current time instance + 0.9. The next time instance is current time instance + 1;
//	7) The original solver cannot handle cases when equations return NAN and INF values due to discontinuities and /0. That is why equations provided by user need to
//      make sure that no NAN and INF are returned.
// 8) Application reads initial data and parameters from text files: y.txt and params.txt respectively that need to be located in the same folder as source files.
//     For simplicity and testing purposes only, when multiple number of simulation instances is specified, application still reads initial data from the same input files. That
//     can be modified in this source code.

//====================================================================================================100
//		IMPLEMENTATION-SPECIFIC DESCRIPTION (OPEN MP)
//====================================================================================================100

// This is the OpenMP version of Myocyte code.

// The original single-threaded code was written in MATLAB and used MATLAB ode45 ODE solver. In the process of accelerating this code, we arrived with the
// intermediate versions that used single-threaded Sundials CVODE solver which evaluated model parallelized with OpenMP at each time step. In order to convert entire
// solver to OpenMP code (to remove some of the operational overheads such as thread launches in OpenMP) we used a simpler solver, from Mathematics Source
// Library, and tailored it to our needs. The parallelism in the cardiac myocyte model is on a very fine-grained level, close to that of ILP, therefore it is very hard to exploit
// as DLP or TLB in OpenMP code. We were able to divide the model into 4 individual groups that run in parallel. However, even that is not enough work to compensate
// for some of the OpenMP thread launch overheads, which resulted in performance worse than that of single-threaded C code. Speedup in this code could
// be achieved only if a customizable accelerator such as FPGA was used for evaluation of the model itself. We also approached the application from another angle and
// allowed it to run several concurrent simulations, thus turning it into an embarrassingly parallel problem. This version of the code is also useful for scientists who want to
// run the same simulation with different sets of input parameters. OpenMP version of this code provides constant speedup of about 3.64x regardless of the number of
// concurrent simulations.

// Speedup numbers reported in the description of this application were obtained on the machine with: Intel Quad Core CPU, 4GB of RAM, Nvidia GTX280 GPU.

// 1) When running with parallelization inside each simulation instance (value of 3rd command line parameter equal to 0), performance is bad because:
// a) thread launch overhead
// b) small amount of work for each forked thread
// 2) When running with parallelization across simulation instances, code gets continues speedup with the increasing number of simulation insances which saturates
//     around 4 instances on Quad Core CPU (roughly corresponding to the number of multiprocessorsXprocessors in GTX280), with the speedup of around 3.5x compared
//     to serial C version of code, as expected.

// The following are the command parameters to the application:
// 1) Simulation time interval which is the number of miliseconds to simulate. Needs to be integer > 0
// 2) Number of instances of simulation to run. Needs to be integer > 0.
// 3) Method of parallelization. Need to be 0 for parallelization inside each simulation instance, or 1 for parallelization across instances.
// 4) Number of threads to use. Needs to be integer > 0.
// Example:
// a.out 100 100 1 4

//====================================================================================================100
//	DEFINE / INCLUDE
//====================================================================================================100

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>

#define fp float

#define EQUATIONS 91
#define PARAMETERS 16
#define ATTEMPTS 12
#define MIN_SCALE_FACTOR 0.125
#define MAX_SCALE_FACTOR 4.0

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

void ecc(fp timeinst,
         fp *initvalu,
         int initvalu_offset,
         fp *parameter,
         int parameter_offset,
         fp *finavalu)
{

    //=====================================================================
    //	VARIABLES
    //=====================================================================

    // initial data and output data variable references
    int offset_1;
    int offset_2;
    int offset_3;
    int offset_4;
    int offset_5;
    int offset_6;
    int offset_7;
    int offset_8;
    int offset_9;
    int offset_10;
    int offset_11;
    int offset_12;
    int offset_13;
    int offset_14;
    int offset_15;
    int offset_16;
    int offset_17;
    int offset_18;
    int offset_19;
    int offset_20;
    int offset_21;
    int offset_22;
    int offset_23;
    int offset_24;
    int offset_25;
    int offset_26;
    int offset_27;
    int offset_28;
    int offset_29;
    int offset_30;
    int offset_31;
    int offset_32;
    int offset_33;
    int offset_34;
    int offset_35;
    int offset_36;
    int offset_37;
    int offset_38;
    int offset_39;
    int offset_40;
    int offset_41;
    int offset_42;
    int offset_43;
    int offset_44;
    int offset_45;
    int offset_46;

    // initial data variable references
    int parameter_offset_1;

    // decoded input initial data			// GET VARIABLES FROM MEMORY AND SAVE LOCALLY !!!!!!!!!!!!!!!!!!
    fp initvalu_1;
    fp initvalu_2;
    fp initvalu_3;
    fp initvalu_4;
    fp initvalu_5;
    fp initvalu_6;
    fp initvalu_7;
    fp initvalu_8;
    fp initvalu_9;
    fp initvalu_10;
    fp initvalu_11;
    fp initvalu_12;
    fp initvalu_13;
    fp initvalu_14;
    fp initvalu_15;
    fp initvalu_16;
    fp initvalu_17;
    fp initvalu_18;
    fp initvalu_19;
    fp initvalu_20;
    fp initvalu_21;
    fp initvalu_22;
    fp initvalu_23;
    fp initvalu_24;
    fp initvalu_25;
    fp initvalu_26;
    fp initvalu_27;
    fp initvalu_28;
    fp initvalu_29;
    fp initvalu_30;
    fp initvalu_31;
    fp initvalu_32;
    fp initvalu_33;
    fp initvalu_34;
    fp initvalu_35;
    fp initvalu_36;
    fp initvalu_37;
    fp initvalu_38;
    fp initvalu_39;
    fp initvalu_40;
    fp initvalu_41;
    fp initvalu_42;
    fp initvalu_43;
    fp initvalu_44;
    fp initvalu_45;
    fp initvalu_46;

    // decoded input parameters
    fp parameter_1;

    // matlab constants undefined in c
    fp pi;

    // Constants
    fp R;    // [J/kmol*K]
    fp Frdy; // [C/mol]
    fp Temp; // [K] 310
    fp FoRT; //
    fp Cmem; // [F] membrane capacitance
    fp Qpow;

    // Cell geometry
    fp cellLength;     // cell length [um]
    fp cellRadius;     // cell radius [um]
    fp junctionLength; // junc length [um]
    fp junctionRadius; // junc radius [um]
    fp distSLcyto;     // dist. SL to cytosol [um]
    fp distJuncSL;     // dist. junc to SL [um]
    fp DcaJuncSL;      // Dca junc to SL [cm^2/sec]
    fp DcaSLcyto;      // Dca SL to cyto [cm^2/sec]
    fp DnaJuncSL;      // Dna junc to SL [cm^2/sec]
    fp DnaSLcyto;      // Dna SL to cyto [cm^2/sec]
    fp Vcell;          // [L]
    fp Vmyo;
    fp Vsr;
    fp Vsl;
    fp Vjunc;
    fp SAjunc;      // [um^2]
    fp SAsl;        // [um^2]
    fp J_ca_juncsl; // [L/msec]
    fp J_ca_slmyo;  // [L/msec]
    fp J_na_juncsl; // [L/msec]
    fp J_na_slmyo;  // [L/msec]

    // Fractional currents in compartments
    fp Fjunc;
    fp Fsl;
    fp Fjunc_CaL;
    fp Fsl_CaL;

    // Fixed ion concentrations
    fp Cli; // Intracellular Cl  [mM]
    fp Clo; // Extracellular Cl  [mM]
    fp Ko;  // Extracellular K   [mM]
    fp Nao; // Extracellular Na  [mM]
    fp Cao; // Extracellular Ca  [mM]
    fp Mgi; // Intracellular Mg  [mM]

    // Nernst Potentials
    fp ena_junc; // [mV]
    fp ena_sl;   // [mV]
    fp ek;       // [mV]
    fp eca_junc; // [mV]
    fp eca_sl;   // [mV]
    fp ecl;      // [mV]

    // Na transport parameters
    fp GNa;     // [mS/uF]
    fp GNaB;    // [mS/uF]
    fp IbarNaK; // [uA/uF]
    fp KmNaip;  // [mM]
    fp KmKo;    // [mM]
    fp Q10NaK;
    fp Q10KmNai;

    // K current parameters
    fp pNaK;
    fp GtoSlow; // [mS/uF]
    fp GtoFast; // [mS/uF]
    fp gkp;

    // Cl current parameters
    fp GClCa;  // [mS/uF]
    fp GClB;   // [mS/uF]
    fp KdClCa; // [mM]																// [mM]

    // I_Ca parameters
    fp pNa;  // [cm/sec]
    fp pCa;  // [cm/sec]
    fp pK;   // [cm/sec]
    fp KmCa; // [mM]
    fp Q10CaL;

    // Ca transport parameters
    fp IbarNCX;   // [uA/uF]
    fp KmCai;     // [mM]
    fp KmCao;     // [mM]
    fp KmNai;     // [mM]
    fp KmNao;     // [mM]
    fp ksat;      // [none]
    fp nu;        // [none]
    fp Kdact;     // [mM]
    fp Q10NCX;    // [none]
    fp IbarSLCaP; // [uA/uF]
    fp KmPCa;     // [mM]
    fp GCaB;      // [uA/uF]
    fp Q10SLCaP;  // [none]																	// [none]

    // SR flux parameters
    fp Q10SRCaP;   // [none]
    fp Vmax_SRCaP; // [mM/msec] (mmol/L cytosol/msec)
    fp Kmf;        // [mM]
    fp Kmr;        // [mM]L cytosol
    fp hillSRCaP;  // [mM]
    fp ks;         // [1/ms]
    fp koCa;       // [mM^-2 1/ms]
    fp kom;        // [1/ms]
    fp kiCa;       // [1/mM/ms]
    fp kim;        // [1/ms]
    fp ec50SR;     // [mM]

    // Buffering parameters
    fp Bmax_Naj;      // [mM]
    fp Bmax_Nasl;     // [mM]
    fp koff_na;       // [1/ms]
    fp kon_na;        // [1/mM/ms]
    fp Bmax_TnClow;   // [mM], TnC low affinity
    fp koff_tncl;     // [1/ms]
    fp kon_tncl;      // [1/mM/ms]
    fp Bmax_TnChigh;  // [mM], TnC high affinity
    fp koff_tnchca;   // [1/ms]
    fp kon_tnchca;    // [1/mM/ms]
    fp koff_tnchmg;   // [1/ms]
    fp kon_tnchmg;    // [1/mM/ms]
    fp Bmax_CaM;      // [mM], CaM buffering
    fp koff_cam;      // [1/ms]
    fp kon_cam;       // [1/mM/ms]
    fp Bmax_myosin;   // [mM], Myosin buffering
    fp koff_myoca;    // [1/ms]
    fp kon_myoca;     // [1/mM/ms]
    fp koff_myomg;    // [1/ms]
    fp kon_myomg;     // [1/mM/ms]
    fp Bmax_SR;       // [mM]
    fp koff_sr;       // [1/ms]
    fp kon_sr;        // [1/mM/ms]
    fp Bmax_SLlowsl;  // [mM], SL buffering
    fp Bmax_SLlowj;   // [mM]
    fp koff_sll;      // [1/ms]
    fp kon_sll;       // [1/mM/ms]
    fp Bmax_SLhighsl; // [mM]
    fp Bmax_SLhighj;  // [mM]
    fp koff_slh;      // [1/ms]
    fp kon_slh;       // [1/mM/ms]
    fp Bmax_Csqn;     // 140e-3*Vmyo/Vsr; [mM]
    fp koff_csqn;     // [1/ms]
    fp kon_csqn;      // [1/mM/ms]

    // I_Na: Fast Na Current
    fp am;
    fp bm;
    fp ah;
    fp bh;
    fp aj;
    fp bj;
    fp I_Na_junc;
    fp I_Na_sl;
    fp I_Na;

    // I_nabk: Na Background Current
    fp I_nabk_junc;
    fp I_nabk_sl;
    fp I_nabk;

    // I_nak: Na/K Pump Current
    fp sigma;
    fp fnak;
    fp I_nak_junc;
    fp I_nak_sl;
    fp I_nak;

    // I_kr: Rapidly Activating K Current
    fp gkr;
    fp xrss;
    fp tauxr;
    fp rkr;
    fp I_kr;

    // I_ks: Slowly Activating K Current
    fp pcaks_junc;
    fp pcaks_sl;
    fp gks_junc;
    fp gks_sl;
    fp eks;
    fp xsss;
    fp tauxs;
    fp I_ks_junc;
    fp I_ks_sl;
    fp I_ks;

    // I_kp: Plateau K current
    fp kp_kp;
    fp I_kp_junc;
    fp I_kp_sl;
    fp I_kp;

    // I_to: Transient Outward K Current (slow and fast components)
    fp xtoss;
    fp ytoss;
    fp rtoss;
    fp tauxtos;
    fp tauytos;
    fp taurtos;
    fp I_tos;

    //
    fp tauxtof;
    fp tauytof;
    fp I_tof;
    fp I_to;

    // I_ki: Time-Independent K Current
    fp aki;
    fp bki;
    fp kiss;
    fp I_ki;

    // I_ClCa: Ca-activated Cl Current, I_Clbk: background Cl Current
    fp I_ClCa_junc;
    fp I_ClCa_sl;
    fp I_ClCa;
    fp I_Clbk;

    // I_Ca: L-type Calcium Current
    fp dss;
    fp taud;
    fp fss;
    fp tauf;

    //
    fp ibarca_j;
    fp ibarca_sl;
    fp ibark;
    fp ibarna_j;
    fp ibarna_sl;
    fp I_Ca_junc;
    fp I_Ca_sl;
    fp I_Ca;
    fp I_CaK;
    fp I_CaNa_junc;
    fp I_CaNa_sl;
    fp I_CaNa;
    fp I_Catot;

    // I_ncx: Na/Ca Exchanger flux
    fp Ka_junc;
    fp Ka_sl;
    fp s1_junc;
    fp s1_sl;
    fp s2_junc;
    fp s3_junc;
    fp s2_sl;
    fp s3_sl;
    fp I_ncx_junc;
    fp I_ncx_sl;
    fp I_ncx;

    // I_pca: Sarcolemmal Ca Pump Current
    fp I_pca_junc;
    fp I_pca_sl;
    fp I_pca;

    // I_cabk: Ca Background Current
    fp I_cabk_junc;
    fp I_cabk_sl;
    fp I_cabk;

    // SR fluxes: Calcium Release, SR Ca pump, SR Ca leak
    fp MaxSR;
    fp MinSR;
    fp kCaSR;
    fp koSRCa;
    fp kiSRCa;
    fp RI;
    fp J_SRCarel; // [mM/ms]
    fp J_serca;
    fp J_SRleak; //   [mM/ms]

    // Cytosolic Ca Buffers
    fp J_CaB_cytosol;

    // Junctional and SL Ca Buffers
    fp J_CaB_junction;
    fp J_CaB_sl;

    // SR Ca Concentrations
    fp oneovervsr;

    // Sodium Concentrations
    fp I_Na_tot_junc; // [uA/uF]
    fp I_Na_tot_sl;   // [uA/uF]
    fp oneovervsl;

    // Potassium Concentration
    fp I_K_tot;

    // Calcium Concentrations
    fp I_Ca_tot_junc; // [uA/uF]
    fp I_Ca_tot_sl;   // [uA/uF]
    fp junc_sl;
    fp sl_junc;
    fp sl_myo;
    fp myo_sl;

    //	Simulation type
    int state; // 0-none; 1-pace; 2-vclamp
    fp I_app;
    fp V_hold;
    fp V_test;
    fp V_clamp;
    fp R_clamp;

    //	Membrane Potential
    fp I_Na_tot; // [uA/uF]
    fp I_Cl_tot; // [uA/uF]
    fp I_Ca_tot;
    fp I_tot;

    //=====================================================================
    //	EXECUTION
    //=====================================================================

    // variable references
    offset_1 = initvalu_offset;
    offset_2 = initvalu_offset + 1;
    offset_3 = initvalu_offset + 2;
    offset_4 = initvalu_offset + 3;
    offset_5 = initvalu_offset + 4;
    offset_6 = initvalu_offset + 5;
    offset_7 = initvalu_offset + 6;
    offset_8 = initvalu_offset + 7;
    offset_9 = initvalu_offset + 8;
    offset_10 = initvalu_offset + 9;
    offset_11 = initvalu_offset + 10;
    offset_12 = initvalu_offset + 11;
    offset_13 = initvalu_offset + 12;
    offset_14 = initvalu_offset + 13;
    offset_15 = initvalu_offset + 14;
    offset_16 = initvalu_offset + 15;
    offset_17 = initvalu_offset + 16;
    offset_18 = initvalu_offset + 17;
    offset_19 = initvalu_offset + 18;
    offset_20 = initvalu_offset + 19;
    offset_21 = initvalu_offset + 20;
    offset_22 = initvalu_offset + 21;
    offset_23 = initvalu_offset + 22;
    offset_24 = initvalu_offset + 23;
    offset_25 = initvalu_offset + 24;
    offset_26 = initvalu_offset + 25;
    offset_27 = initvalu_offset + 26;
    offset_28 = initvalu_offset + 27;
    offset_29 = initvalu_offset + 28;
    offset_30 = initvalu_offset + 29;
    offset_31 = initvalu_offset + 30;
    offset_32 = initvalu_offset + 31;
    offset_33 = initvalu_offset + 32;
    offset_34 = initvalu_offset + 33;
    offset_35 = initvalu_offset + 34;
    offset_36 = initvalu_offset + 35;
    offset_37 = initvalu_offset + 36;
    offset_38 = initvalu_offset + 37;
    offset_39 = initvalu_offset + 38;
    offset_40 = initvalu_offset + 39;
    offset_41 = initvalu_offset + 40;
    offset_42 = initvalu_offset + 41;
    offset_43 = initvalu_offset + 42;
    offset_44 = initvalu_offset + 43;
    offset_45 = initvalu_offset + 44;
    offset_46 = initvalu_offset + 45;

    // variable references
    parameter_offset_1 = parameter_offset;

    // decoded input initial data
    initvalu_1 = initvalu[offset_1];
    initvalu_2 = initvalu[offset_2];
    initvalu_3 = initvalu[offset_3];
    initvalu_4 = initvalu[offset_4];
    initvalu_5 = initvalu[offset_5];
    initvalu_6 = initvalu[offset_6];
    initvalu_7 = initvalu[offset_7];
    initvalu_8 = initvalu[offset_8];
    initvalu_9 = initvalu[offset_9];
    initvalu_10 = initvalu[offset_10];
    initvalu_11 = initvalu[offset_11];
    initvalu_12 = initvalu[offset_12];
    initvalu_13 = initvalu[offset_13];
    initvalu_14 = initvalu[offset_14];
    initvalu_15 = initvalu[offset_15];
    initvalu_16 = initvalu[offset_16];
    initvalu_17 = initvalu[offset_17];
    initvalu_18 = initvalu[offset_18];
    initvalu_19 = initvalu[offset_19];
    initvalu_20 = initvalu[offset_20];
    initvalu_21 = initvalu[offset_21];
    initvalu_22 = initvalu[offset_22];
    initvalu_23 = initvalu[offset_23];
    initvalu_24 = initvalu[offset_24];
    initvalu_25 = initvalu[offset_25];
    initvalu_26 = initvalu[offset_26];
    initvalu_27 = initvalu[offset_27];
    initvalu_28 = initvalu[offset_28];
    initvalu_29 = initvalu[offset_29];
    initvalu_30 = initvalu[offset_30];
    initvalu_31 = initvalu[offset_31];
    initvalu_32 = initvalu[offset_32];
    initvalu_33 = initvalu[offset_33];
    initvalu_34 = initvalu[offset_34];
    initvalu_35 = initvalu[offset_35];
    initvalu_36 = initvalu[offset_36];
    initvalu_37 = initvalu[offset_37];
    initvalu_38 = initvalu[offset_38];
    initvalu_39 = initvalu[offset_39];
    initvalu_40 = initvalu[offset_40];
    initvalu_41 = initvalu[offset_41];
    initvalu_42 = initvalu[offset_42];
    initvalu_43 = initvalu[offset_43];
    initvalu_44 = initvalu[offset_44];
    initvalu_45 = initvalu[offset_45];
    initvalu_46 = initvalu[offset_46];

    // decoded input parameters
    parameter_1 = parameter[parameter_offset_1];

    // matlab constants undefined in c
    pi = 3.1416;

    // Constants
    R = 8314;               // [J/kmol*K]
    Frdy = 96485;           // [C/mol]
    Temp = 310;             // [K] 310
    FoRT = Frdy / R / Temp; //
    Cmem = 1.3810e-10;      // [F] membrane capacitance
    Qpow = (Temp - 310) / 10;

    // Cell geometry
    cellLength = 100;                                     // cell length [um]
    cellRadius = 10.25;                                   // cell radius [um]
    junctionLength = 160e-3;                              // junc length [um]
    junctionRadius = 15e-3;                               // junc radius [um]
    distSLcyto = 0.45;                                    // dist. SL to cytosol [um]
    distJuncSL = 0.5;                                     // dist. junc to SL [um]
    DcaJuncSL = 1.64e-6;                                  // Dca junc to SL [cm^2/sec]
    DcaSLcyto = 1.22e-6;                                  // Dca SL to cyto [cm^2/sec]
    DnaJuncSL = 1.09e-5;                                  // Dna junc to SL [cm^2/sec]
    DnaSLcyto = 1.79e-5;                                  // Dna SL to cyto [cm^2/sec]
    Vcell = pi * pow(cellRadius, 2) * cellLength * 1e-15; // [L]
    Vmyo = 0.65 * Vcell;
    Vsr = 0.035 * Vcell;
    Vsl = 0.02 * Vcell;
    Vjunc = 0.0539 * 0.01 * Vcell;
    SAjunc = 20150 * pi * 2 * junctionLength * junctionRadius; // [um^2]
    SAsl = pi * 2 * cellRadius * cellLength;                   // [um^2]
    J_ca_juncsl = 1 / 1.2134e12;                               // [L/msec]
    J_ca_slmyo = 1 / 2.68510e11;                               // [L/msec]
    J_na_juncsl = 1 / (1.6382e12 / 3 * 100);                   // [L/msec]
    J_na_slmyo = 1 / (1.8308e10 / 3 * 100);                    // [L/msec]

    // Fractional currents in compartments
    Fjunc = 0.11;
    Fsl = 1 - Fjunc;
    Fjunc_CaL = 0.9;
    Fsl_CaL = 1 - Fjunc_CaL;

    // Fixed ion concentrations
    Cli = 15;  // Intracellular Cl  [mM]
    Clo = 150; // Extracellular Cl  [mM]
    Ko = 5.4;  // Extracellular K   [mM]
    Nao = 140; // Extracellular Na  [mM]
    Cao = 1.8; // Extracellular Ca  [mM]
    Mgi = 1;   // Intracellular Mg  [mM]

    // Nernst Potentials
    ena_junc = (1 / FoRT) * log(Nao / initvalu_32);     // [mV]
    ena_sl = (1 / FoRT) * log(Nao / initvalu_33);       // [mV]
    ek = (1 / FoRT) * log(Ko / initvalu_35);            // [mV]
    eca_junc = (1 / FoRT / 2) * log(Cao / initvalu_36); // [mV]
    eca_sl = (1 / FoRT / 2) * log(Cao / initvalu_37);   // [mV]
    ecl = (1 / FoRT) * log(Cli / Clo);                  // [mV]

    // Na transport parameters
    GNa = 16.0;        // [mS/uF]
    GNaB = 0.297e-3;   // [mS/uF]
    IbarNaK = 1.90719; // [uA/uF]
    KmNaip = 11;       // [mM]
    KmKo = 1.5;        // [mM]
    Q10NaK = 1.63;
    Q10KmNai = 1.39;

    // K current parameters
    pNaK = 0.01833;
    GtoSlow = 0.06; // [mS/uF]
    GtoFast = 0.02; // [mS/uF]
    gkp = 0.001;

    // Cl current parameters
    GClCa = 0.109625; // [mS/uF]
    GClB = 9e-3;      // [mS/uF]
    KdClCa = 100e-3;  // [mM]

    // I_Ca parameters
    pNa = 1.5e-8;  // [cm/sec]
    pCa = 5.4e-4;  // [cm/sec]
    pK = 2.7e-7;   // [cm/sec]
    KmCa = 0.6e-3; // [mM]
    Q10CaL = 1.8;

    // Ca transport parameters
    IbarNCX = 9.0;      // [uA/uF]
    KmCai = 3.59e-3;    // [mM]
    KmCao = 1.3;        // [mM]
    KmNai = 12.29;      // [mM]
    KmNao = 87.5;       // [mM]
    ksat = 0.27;        // [none]
    nu = 0.35;          // [none]
    Kdact = 0.256e-3;   // [mM]
    Q10NCX = 1.57;      // [none]
    IbarSLCaP = 0.0673; // [uA/uF]
    KmPCa = 0.5e-3;     // [mM]
    GCaB = 2.513e-4;    // [uA/uF]
    Q10SLCaP = 2.35;    // [none]

    // SR flux parameters
    Q10SRCaP = 2.6;       // [none]
    Vmax_SRCaP = 2.86e-4; // [mM/msec] (mmol/L cytosol/msec)
    Kmf = 0.246e-3;       // [mM]
    Kmr = 1.7;            // [mM]L cytosol
    hillSRCaP = 1.787;    // [mM]
    ks = 25;              // [1/ms]
    koCa = 10;            // [mM^-2 1/ms]
    kom = 0.06;           // [1/ms]
    kiCa = 0.5;           // [1/mM/ms]
    kim = 0.005;          // [1/ms]
    ec50SR = 0.45;        // [mM]

    // Buffering parameters
    Bmax_Naj = 7.561;                            // [mM]
    Bmax_Nasl = 1.65;                            // [mM]
    koff_na = 1e-3;                              // [1/ms]
    kon_na = 0.1e-3;                             // [1/mM/ms]
    Bmax_TnClow = 70e-3;                         // [mM], TnC low affinity
    koff_tncl = 19.6e-3;                         // [1/ms]
    kon_tncl = 32.7;                             // [1/mM/ms]
    Bmax_TnChigh = 140e-3;                       // [mM], TnC high affinity
    koff_tnchca = 0.032e-3;                      // [1/ms]
    kon_tnchca = 2.37;                           // [1/mM/ms]
    koff_tnchmg = 3.33e-3;                       // [1/ms]
    kon_tnchmg = 3e-3;                           // [1/mM/ms]
    Bmax_CaM = 24e-3;                            // [mM], CaM buffering
    koff_cam = 238e-3;                           // [1/ms]
    kon_cam = 34;                                // [1/mM/ms]
    Bmax_myosin = 140e-3;                        // [mM], Myosin buffering
    koff_myoca = 0.46e-3;                        // [1/ms]
    kon_myoca = 13.8;                            // [1/mM/ms]
    koff_myomg = 0.057e-3;                       // [1/ms]
    kon_myomg = 0.0157;                          // [1/mM/ms]
    Bmax_SR = 19 * 0.9e-3;                       // [mM]
    koff_sr = 60e-3;                             // [1/ms]
    kon_sr = 100;                                // [1/mM/ms]
    Bmax_SLlowsl = 37.38e-3 * Vmyo / Vsl;        // [mM], SL buffering
    Bmax_SLlowj = 4.62e-3 * Vmyo / Vjunc * 0.1;  // [mM]
    koff_sll = 1300e-3;                          // [1/ms]
    kon_sll = 100;                               // [1/mM/ms]
    Bmax_SLhighsl = 13.35e-3 * Vmyo / Vsl;       // [mM]
    Bmax_SLhighj = 1.65e-3 * Vmyo / Vjunc * 0.1; // [mM]
    koff_slh = 30e-3;                            // [1/ms]
    kon_slh = 100;                               // [1/mM/ms]
    Bmax_Csqn = 2.7;                             // 140e-3*Vmyo/Vsr; [mM]
    koff_csqn = 65;                              // [1/ms]
    kon_csqn = 100;                              // [1/mM/ms]

    // I_Na: Fast Na Current
    am = 0.32 * (initvalu_39 + 47.13) / (1 - exp(-0.1 * (initvalu_39 + 47.13)));
    bm = 0.08 * exp(-initvalu_39 / 11);
    if (initvalu_39 >= -40)
    {
        ah = 0;
        aj = 0;
        bh = 1 / (0.13 * (1 + exp(-(initvalu_39 + 10.66) / 11.1)));
        bj = 0.3 * exp(-2.535e-7 * initvalu_39) / (1 + exp(-0.1 * (initvalu_39 + 32)));
    }
    else
    {
        ah = 0.135 * exp((80 + initvalu_39) / -6.8);
        bh = 3.56 * exp(0.079 * initvalu_39) + 3.1e5 * exp(0.35 * initvalu_39);
        aj = (-127140 * exp(0.2444 * initvalu_39) - 3.474e-5 * exp(-0.04391 * initvalu_39)) * (initvalu_39 + 37.78) / (1 + exp(0.311 * (initvalu_39 + 79.23)));
        bj = 0.1212 * exp(-0.01052 * initvalu_39) / (1 + exp(-0.1378 * (initvalu_39 + 40.14)));
    }
    finavalu[offset_1] = am * (1 - initvalu_1) - bm * initvalu_1;
    finavalu[offset_2] = ah * (1 - initvalu_2) - bh * initvalu_2;
    finavalu[offset_3] = aj * (1 - initvalu_3) - bj * initvalu_3;
    I_Na_junc = Fjunc * GNa * pow(initvalu_1, 3) * initvalu_2 * initvalu_3 * (initvalu_39 - ena_junc);
    I_Na_sl = Fsl * GNa * pow(initvalu_1, 3) * initvalu_2 * initvalu_3 * (initvalu_39 - ena_sl);
    I_Na = I_Na_junc + I_Na_sl;

    // I_nabk: Na Background Current
    I_nabk_junc = Fjunc * GNaB * (initvalu_39 - ena_junc);
    I_nabk_sl = Fsl * GNaB * (initvalu_39 - ena_sl);
    I_nabk = I_nabk_junc + I_nabk_sl;

    // I_nak: Na/K Pump Current
    sigma = (exp(Nao / 67.3) - 1) / 7;
    fnak = 1 / (1 + 0.1245 * exp(-0.1 * initvalu_39 * FoRT) + 0.0365 * sigma * exp(-initvalu_39 * FoRT));
    I_nak_junc = Fjunc * IbarNaK * fnak * Ko / (1 + pow((KmNaip / initvalu_32), 4)) / (Ko + KmKo);
    I_nak_sl = Fsl * IbarNaK * fnak * Ko / (1 + pow((KmNaip / initvalu_33), 4)) / (Ko + KmKo);
    I_nak = I_nak_junc + I_nak_sl;

    // I_kr: Rapidly Activating K Current
    gkr = 0.03 * sqrt(Ko / 5.4);
    xrss = 1 / (1 + exp(-(initvalu_39 + 50) / 7.5));
    tauxr = 1 / (0.00138 * (initvalu_39 + 7) / (1 - exp(-0.123 * (initvalu_39 + 7))) + 6.1e-4 * (initvalu_39 + 10) / (exp(0.145 * (initvalu_39 + 10)) - 1));
    finavalu[offset_12] = (xrss - initvalu_12) / tauxr;
    rkr = 1 / (1 + exp((initvalu_39 + 33) / 22.4));
    I_kr = gkr * initvalu_12 * rkr * (initvalu_39 - ek);

    // I_ks: Slowly Activating K Current
    pcaks_junc = -log10(initvalu_36) + 3.0;
    pcaks_sl = -log10(initvalu_37) + 3.0;
    gks_junc = 0.07 * (0.057 + 0.19 / (1 + exp((-7.2 + pcaks_junc) / 0.6)));
    gks_sl = 0.07 * (0.057 + 0.19 / (1 + exp((-7.2 + pcaks_sl) / 0.6)));
    eks = (1 / FoRT) * log((Ko + pNaK * Nao) / (initvalu_35 + pNaK * initvalu_34));
    xsss = 1 / (1 + exp(-(initvalu_39 - 1.5) / 16.7));
    tauxs = 1 / (7.19e-5 * (initvalu_39 + 30) / (1 - exp(-0.148 * (initvalu_39 + 30))) + 1.31e-4 * (initvalu_39 + 30) / (exp(0.0687 * (initvalu_39 + 30)) - 1));
    finavalu[offset_13] = (xsss - initvalu_13) / tauxs;
    I_ks_junc = Fjunc * gks_junc * pow(initvalu_12, 2) * (initvalu_39 - eks);
    I_ks_sl = Fsl * gks_sl * pow(initvalu_13, 2) * (initvalu_39 - eks);
    I_ks = I_ks_junc + I_ks_sl;

    // I_kp: Plateau K current
    kp_kp = 1 / (1 + exp(7.488 - initvalu_39 / 5.98));
    I_kp_junc = Fjunc * gkp * kp_kp * (initvalu_39 - ek);
    I_kp_sl = Fsl * gkp * kp_kp * (initvalu_39 - ek);
    I_kp = I_kp_junc + I_kp_sl;

    // I_to: Transient Outward K Current (slow and fast components)
    xtoss = 1 / (1 + exp(-(initvalu_39 + 3.0) / 15));
    ytoss = 1 / (1 + exp((initvalu_39 + 33.5) / 10));
    rtoss = 1 / (1 + exp((initvalu_39 + 33.5) / 10));
    tauxtos = 9 / (1 + exp((initvalu_39 + 3.0) / 15)) + 0.5;
    tauytos = 3e3 / (1 + exp((initvalu_39 + 60.0) / 10)) + 30;
    taurtos = 2800 / (1 + exp((initvalu_39 + 60.0) / 10)) + 220;
    finavalu[offset_8] = (xtoss - initvalu_8) / tauxtos;
    finavalu[offset_9] = (ytoss - initvalu_9) / tauytos;
    finavalu[offset_40] = (rtoss - initvalu_40) / taurtos;
    I_tos = GtoSlow * initvalu_8 * (initvalu_9 + 0.5 * initvalu_40) * (initvalu_39 - ek); // [uA/uF]

    //
    tauxtof = 3.5 * exp(-initvalu_39 * initvalu_39 / 30 / 30) + 1.5;
    tauytof = 20.0 / (1 + exp((initvalu_39 + 33.5) / 10)) + 20.0;
    finavalu[offset_10] = (xtoss - initvalu_10) / tauxtof;
    finavalu[offset_11] = (ytoss - initvalu_11) / tauytof;
    I_tof = GtoFast * initvalu_10 * initvalu_11 * (initvalu_39 - ek);
    I_to = I_tos + I_tof;

    // I_ki: Time-Independent K Current
    aki = 1.02 / (1 + exp(0.2385 * (initvalu_39 - ek - 59.215)));
    bki = (0.49124 * exp(0.08032 * (initvalu_39 + 5.476 - ek)) + exp(0.06175 * (initvalu_39 - ek - 594.31))) / (1 + exp(-0.5143 * (initvalu_39 - ek + 4.753)));
    kiss = aki / (aki + bki);
    I_ki = 0.9 * sqrt(Ko / 5.4) * kiss * (initvalu_39 - ek);

    // I_ClCa: Ca-activated Cl Current, I_Clbk: background Cl Current
    I_ClCa_junc = Fjunc * GClCa / (1 + KdClCa / initvalu_36) * (initvalu_39 - ecl);
    I_ClCa_sl = Fsl * GClCa / (1 + KdClCa / initvalu_37) * (initvalu_39 - ecl);
    I_ClCa = I_ClCa_junc + I_ClCa_sl;
    I_Clbk = GClB * (initvalu_39 - ecl);

    // I_Ca: L-type Calcium Current
    dss = 1 / (1 + exp(-(initvalu_39 + 14.5) / 6.0));
    taud = dss * (1 - exp(-(initvalu_39 + 14.5) / 6.0)) / (0.035 * (initvalu_39 + 14.5));
    fss = 1 / (1 + exp((initvalu_39 + 35.06) / 3.6)) + 0.6 / (1 + exp((50 - initvalu_39) / 20));
    tauf = 1 / (0.0197 * exp(-pow(0.0337 * (initvalu_39 + 14.5), 2)) + 0.02);
    finavalu[offset_4] = (dss - initvalu_4) / taud;
    finavalu[offset_5] = (fss - initvalu_5) / tauf;
    finavalu[offset_6] = 1.7 * initvalu_36 * (1 - initvalu_6) - 11.9e-3 * initvalu_6; // fCa_junc
    finavalu[offset_7] = 1.7 * initvalu_37 * (1 - initvalu_7) - 11.9e-3 * initvalu_7; // fCa_sl

    //
    ibarca_j = pCa * 4 * (initvalu_39 * Frdy * FoRT) * (0.341 * initvalu_36 * exp(2 * initvalu_39 * FoRT) - 0.341 * Cao) / (exp(2 * initvalu_39 * FoRT) - 1);
    ibarca_sl = pCa * 4 * (initvalu_39 * Frdy * FoRT) * (0.341 * initvalu_37 * exp(2 * initvalu_39 * FoRT) - 0.341 * Cao) / (exp(2 * initvalu_39 * FoRT) - 1);
    ibark = pK * (initvalu_39 * Frdy * FoRT) * (0.75 * initvalu_35 * exp(initvalu_39 * FoRT) - 0.75 * Ko) / (exp(initvalu_39 * FoRT) - 1);
    ibarna_j = pNa * (initvalu_39 * Frdy * FoRT) * (0.75 * initvalu_32 * exp(initvalu_39 * FoRT) - 0.75 * Nao) / (exp(initvalu_39 * FoRT) - 1);
    ibarna_sl = pNa * (initvalu_39 * Frdy * FoRT) * (0.75 * initvalu_33 * exp(initvalu_39 * FoRT) - 0.75 * Nao) / (exp(initvalu_39 * FoRT) - 1);
    I_Ca_junc = (Fjunc_CaL * ibarca_j * initvalu_4 * initvalu_5 * (1 - initvalu_6) * pow(Q10CaL, Qpow)) * 0.45;
    I_Ca_sl = (Fsl_CaL * ibarca_sl * initvalu_4 * initvalu_5 * (1 - initvalu_7) * pow(Q10CaL, Qpow)) * 0.45;
    I_Ca = I_Ca_junc + I_Ca_sl;
    finavalu[offset_43] = -I_Ca * Cmem / (Vmyo * 2 * Frdy) * 1e3;
    I_CaK = (ibark * initvalu_4 * initvalu_5 * (Fjunc_CaL * (1 - initvalu_6) + Fsl_CaL * (1 - initvalu_7)) * pow(Q10CaL, Qpow)) * 0.45;
    I_CaNa_junc = (Fjunc_CaL * ibarna_j * initvalu_4 * initvalu_5 * (1 - initvalu_6) * pow(Q10CaL, Qpow)) * 0.45;
    I_CaNa_sl = (Fsl_CaL * ibarna_sl * initvalu_4 * initvalu_5 * (1 - initvalu_7) * pow(Q10CaL, Qpow)) * 0.45;
    I_CaNa = I_CaNa_junc + I_CaNa_sl;
    I_Catot = I_Ca + I_CaK + I_CaNa;

    // I_ncx: Na/Ca Exchanger flux
    Ka_junc = 1 / (1 + pow((Kdact / initvalu_36), 3));
    Ka_sl = 1 / (1 + pow((Kdact / initvalu_37), 3));
    s1_junc = exp(nu * initvalu_39 * FoRT) * pow(initvalu_32, 3) * Cao;
    s1_sl = exp(nu * initvalu_39 * FoRT) * pow(initvalu_33, 3) * Cao;
    s2_junc = exp((nu - 1) * initvalu_39 * FoRT) * pow(Nao, 3) * initvalu_36;
    s3_junc = (KmCai * pow(Nao, 3) * (1 + pow((initvalu_32 / KmNai), 3)) + pow(KmNao, 3) * initvalu_36 + pow(KmNai, 3) * Cao * (1 + initvalu_36 / KmCai) + KmCao * pow(initvalu_32, 3) + pow(initvalu_32, 3) * Cao + pow(Nao, 3) * initvalu_36) * (1 + ksat * exp((nu - 1) * initvalu_39 * FoRT));
    s2_sl = exp((nu - 1) * initvalu_39 * FoRT) * pow(Nao, 3) * initvalu_37;
    s3_sl = (KmCai * pow(Nao, 3) * (1 + pow((initvalu_33 / KmNai), 3)) + pow(KmNao, 3) * initvalu_37 + pow(KmNai, 3) * Cao * (1 + initvalu_37 / KmCai) + KmCao * pow(initvalu_33, 3) + pow(initvalu_33, 3) * Cao + pow(Nao, 3) * initvalu_37) * (1 + ksat * exp((nu - 1) * initvalu_39 * FoRT));
    I_ncx_junc = Fjunc * IbarNCX * pow(Q10NCX, Qpow) * Ka_junc * (s1_junc - s2_junc) / s3_junc;
    I_ncx_sl = Fsl * IbarNCX * pow(Q10NCX, Qpow) * Ka_sl * (s1_sl - s2_sl) / s3_sl;
    I_ncx = I_ncx_junc + I_ncx_sl;
    finavalu[offset_45] = 2 * I_ncx * Cmem / (Vmyo * 2 * Frdy) * 1e3;

    // I_pca: Sarcolemmal Ca Pump Current
    I_pca_junc = Fjunc *
                 pow(Q10SLCaP, Qpow) *
                 IbarSLCaP *
                 pow(initvalu_36, 1.6) /
                 (pow(KmPCa, 1.6) + pow(initvalu_36, 1.6));
    I_pca_sl = Fsl *
               pow(Q10SLCaP, Qpow) *
               IbarSLCaP *
               pow(initvalu_37, 1.6) /
               (pow(KmPCa, 1.6) + pow(initvalu_37, 1.6));
    I_pca = I_pca_junc + I_pca_sl;
    finavalu[offset_44] = -I_pca * Cmem / (Vmyo * 2 * Frdy) * 1e3;

    // I_cabk: Ca Background Current
    I_cabk_junc = Fjunc * GCaB * (initvalu_39 - eca_junc);
    I_cabk_sl = Fsl * GCaB * (initvalu_39 - eca_sl);
    I_cabk = I_cabk_junc + I_cabk_sl;
    finavalu[offset_46] = -I_cabk * Cmem / (Vmyo * 2 * Frdy) * 1e3;

    // SR fluxes: Calcium Release, SR Ca pump, SR Ca leak
    MaxSR = 15;
    MinSR = 1;
    kCaSR = MaxSR - (MaxSR - MinSR) / (1 + pow(ec50SR / initvalu_31, 2.5));
    koSRCa = koCa / kCaSR;
    kiSRCa = kiCa * kCaSR;
    RI = 1 - initvalu_14 - initvalu_15 - initvalu_16;
    finavalu[offset_14] = (kim * RI - kiSRCa * initvalu_36 * initvalu_14) - (koSRCa * pow(initvalu_36, 2) * initvalu_14 - kom * initvalu_15);          // R
    finavalu[offset_15] = (koSRCa * pow(initvalu_36, 2) * initvalu_14 - kom * initvalu_15) - (kiSRCa * initvalu_36 * initvalu_15 - kim * initvalu_16); // O
    finavalu[offset_16] = (kiSRCa * initvalu_36 * initvalu_15 - kim * initvalu_16) - (kom * initvalu_16 - koSRCa * pow(initvalu_36, 2) * RI);          // I
    J_SRCarel = ks * initvalu_15 * (initvalu_31 - initvalu_36);                                                                                        // [mM/ms]
    J_serca = pow(Q10SRCaP, Qpow) * Vmax_SRCaP * (pow((initvalu_38 / Kmf), hillSRCaP) - pow((initvalu_31 / Kmr), hillSRCaP)) / (1 + pow((initvalu_38 / Kmf), hillSRCaP) + pow((initvalu_31 / Kmr), hillSRCaP));
    J_SRleak = 5.348e-6 * (initvalu_31 - initvalu_36); //   [mM/ms]

    // Sodium and Calcium Buffering
    finavalu[offset_17] = kon_na * initvalu_32 * (Bmax_Naj - initvalu_17) - koff_na * initvalu_17;  // NaBj      [mM/ms]
    finavalu[offset_18] = kon_na * initvalu_33 * (Bmax_Nasl - initvalu_18) - koff_na * initvalu_18; // NaBsl     [mM/ms]

    // Cytosolic Ca Buffers
    finavalu[offset_19] = kon_tncl * initvalu_38 * (Bmax_TnClow - initvalu_19) - koff_tncl * initvalu_19;                    // TnCL      [mM/ms]
    finavalu[offset_20] = kon_tnchca * initvalu_38 * (Bmax_TnChigh - initvalu_20 - initvalu_21) - koff_tnchca * initvalu_20; // TnCHc     [mM/ms]
    finavalu[offset_21] = kon_tnchmg * Mgi * (Bmax_TnChigh - initvalu_20 - initvalu_21) - koff_tnchmg * initvalu_21;         // TnCHm     [mM/ms]
    finavalu[offset_22] = 0;                                                                                                 // CaM       [mM/ms]
    finavalu[offset_23] = kon_myoca * initvalu_38 * (Bmax_myosin - initvalu_23 - initvalu_24) - koff_myoca * initvalu_23;    // Myosin_ca [mM/ms]
    finavalu[offset_24] = kon_myomg * Mgi * (Bmax_myosin - initvalu_23 - initvalu_24) - koff_myomg * initvalu_24;            // Myosin_mg [mM/ms]
    finavalu[offset_25] = kon_sr * initvalu_38 * (Bmax_SR - initvalu_25) - koff_sr * initvalu_25;                            // SRB       [mM/ms]
    J_CaB_cytosol = finavalu[offset_19] + finavalu[offset_20] + finavalu[offset_21] + finavalu[offset_22] + finavalu[offset_23] + finavalu[offset_24] + finavalu[offset_25];

    // Junctional and SL Ca Buffers
    finavalu[offset_26] = kon_sll * initvalu_36 * (Bmax_SLlowj - initvalu_26) - koff_sll * initvalu_26;   // SLLj      [mM/ms]
    finavalu[offset_27] = kon_sll * initvalu_37 * (Bmax_SLlowsl - initvalu_27) - koff_sll * initvalu_27;  // SLLsl     [mM/ms]
    finavalu[offset_28] = kon_slh * initvalu_36 * (Bmax_SLhighj - initvalu_28) - koff_slh * initvalu_28;  // SLHj      [mM/ms]
    finavalu[offset_29] = kon_slh * initvalu_37 * (Bmax_SLhighsl - initvalu_29) - koff_slh * initvalu_29; // SLHsl     [mM/ms]
    J_CaB_junction = finavalu[offset_26] + finavalu[offset_28];
    J_CaB_sl = finavalu[offset_27] + finavalu[offset_29];

    // SR Ca Concentrations
    finavalu[offset_30] = kon_csqn * initvalu_31 * (Bmax_Csqn - initvalu_30) - koff_csqn * initvalu_30; // Csqn      [mM/ms]
    oneovervsr = 1 / Vsr;
    finavalu[offset_31] = J_serca * Vmyo * oneovervsr - (J_SRleak * Vmyo * oneovervsr + J_SRCarel) - finavalu[offset_30]; // Ca_sr     [mM/ms] %Ratio 3 leak current

    // Sodium Concentrations
    I_Na_tot_junc = I_Na_junc + I_nabk_junc + 3 * I_ncx_junc + 3 * I_nak_junc + I_CaNa_junc; // [uA/uF]
    I_Na_tot_sl = I_Na_sl + I_nabk_sl + 3 * I_ncx_sl + 3 * I_nak_sl + I_CaNa_sl;             // [uA/uF]
    finavalu[offset_32] = -I_Na_tot_junc * Cmem / (Vjunc * Frdy) + J_na_juncsl / Vjunc * (initvalu_33 - initvalu_32) - finavalu[offset_17];
    oneovervsl = 1 / Vsl;
    finavalu[offset_33] = -I_Na_tot_sl * Cmem * oneovervsl / Frdy + J_na_juncsl * oneovervsl * (initvalu_32 - initvalu_33) + J_na_slmyo * oneovervsl * (initvalu_34 - initvalu_33) - finavalu[offset_18];
    finavalu[offset_34] = J_na_slmyo / Vmyo * (initvalu_33 - initvalu_34); // [mM/msec]

    // Potassium Concentration
    I_K_tot = I_to + I_kr + I_ks + I_ki - 2 * I_nak + I_CaK + I_kp; // [uA/uF]
    finavalu[offset_35] = 0;                                        // [mM/msec]

    // Calcium Concentrations
    I_Ca_tot_junc = I_Ca_junc + I_cabk_junc + I_pca_junc - 2 * I_ncx_junc;                                                                                                                     // [uA/uF]
    I_Ca_tot_sl = I_Ca_sl + I_cabk_sl + I_pca_sl - 2 * I_ncx_sl;                                                                                                                               // [uA/uF]
    finavalu[offset_36] = -I_Ca_tot_junc * Cmem / (Vjunc * 2 * Frdy) + J_ca_juncsl / Vjunc * (initvalu_37 - initvalu_36) - J_CaB_junction + (J_SRCarel)*Vsr / Vjunc + J_SRleak * Vmyo / Vjunc; // Ca_j
    finavalu[offset_37] = -I_Ca_tot_sl * Cmem / (Vsl * 2 * Frdy) + J_ca_juncsl / Vsl * (initvalu_36 - initvalu_37) + J_ca_slmyo / Vsl * (initvalu_38 - initvalu_37) - J_CaB_sl;                // Ca_sl
    finavalu[offset_38] = -J_serca - J_CaB_cytosol + J_ca_slmyo / Vmyo * (initvalu_37 - initvalu_38);
    junc_sl = J_ca_juncsl / Vsl * (initvalu_36 - initvalu_37);
    sl_junc = J_ca_juncsl / Vjunc * (initvalu_37 - initvalu_36);
    sl_myo = J_ca_slmyo / Vsl * (initvalu_38 - initvalu_37);
    myo_sl = J_ca_slmyo / Vmyo * (initvalu_37 - initvalu_38);

    // Simulation type
    state = 1;
    switch (state)
    {
    case 0:
        I_app = 0;
        break;
    case 1: // pace w/ current injection at cycleLength 'cycleLength'
        if (fmod(timeinst, parameter_1) <= 5)
        {
            I_app = 9.5;
        }
        else
        {
            I_app = 0.0;
        }
        break;
    case 2:
        V_hold = -55;
        V_test = 0;
        if (timeinst > 0.5 & timeinst < 200.5)
        {
            V_clamp = V_test;
        }
        else
        {
            V_clamp = V_hold;
        }
        R_clamp = 0.04;
        I_app = (V_clamp - initvalu_39) / R_clamp;
        break;
    }

    // Membrane Potential
    I_Na_tot = I_Na_tot_junc + I_Na_tot_sl; // [uA/uF]
    I_Cl_tot = I_ClCa + I_Clbk;             // [uA/uF]
    I_Ca_tot = I_Ca_tot_junc + I_Ca_tot_sl;
    I_tot = I_Na_tot + I_Cl_tot + I_Ca_tot + I_K_tot;
    finavalu[offset_39] = -(I_tot - I_app);

    // Set unused output values to 0 (MATLAB does it by default)
    finavalu[offset_41] = 0;
    finavalu[offset_42] = 0;
}

fp cam(fp timeinst,
       fp *initvalu,
       int initvalu_offset,
       fp *parameter,
       int parameter_offset,
       fp *finavalu,
       fp Ca)
{

    //=====================================================================
    //	VARIABLES
    //=====================================================================

    // output
    fp JCa;

    // input data and output data variable references
    int offset_1;
    int offset_2;
    int offset_3;
    int offset_4;
    int offset_5;
    int offset_6;
    int offset_7;
    int offset_8;
    int offset_9;
    int offset_10;
    int offset_11;
    int offset_12;
    int offset_13;
    int offset_14;
    int offset_15;

    // parameter variable references
    int parameter_offset_1;
    int parameter_offset_2;
    int parameter_offset_3;
    int parameter_offset_4;
    int parameter_offset_5;

    // decoding input initial values
    fp CaM;
    fp Ca2CaM;
    fp Ca4CaM;
    fp CaMB;
    fp Ca2CaMB;
    fp Ca4CaMB;
    fp Pb2;
    fp Pb;
    fp Pt;
    fp Pt2;
    fp Pa;
    fp Ca4CaN;
    fp CaMCa4CaN;
    fp Ca2CaMCa4CaN;
    fp Ca4CaMCa4CaN;

    // decoding input parameters
    fp CaMtot;
    fp Btot;
    fp CaMKIItot;
    fp CaNtot;
    fp PP1tot;

    // constants
    fp K;  //
    fp Mg; //

    // Ca/CaM parameters
    fp Kd02; // [uM^2]
    fp Kd24; // [uM^2]
    fp k20;  // [s^-1]
    fp k02;  // [uM^-2 s^-1]
    fp k42;  // [s^-1]
    fp k24;  // [uM^-2 s^-1]

    // CaM buffering (B) parameters
    fp k0Boff; // [s^-1]
    fp k0Bon;  // [uM^-1 s^-1] kon = koff/Kd
    fp k2Boff; // [s^-1]
    fp k2Bon;  // [uM^-1 s^-1]
    fp k4Boff; // [s^-1]
    fp k4Bon;  // [uM^-1 s^-1]

    // using thermodynamic constraints
    fp k20B; // [s^-1] thermo constraint on loop 1
    fp k02B; // [uM^-2 s^-1]
    fp k42B; // [s^-1] thermo constraint on loop 2
    fp k24B; // [uM^-2 s^-1]

    // Wi Wa Wt Wp
    fp kbi;   // [s^-1] (Ca4CaM dissocation from Wb)
    fp kib;   // [uM^-1 s^-1]
    fp kpp1;  // [s^-1] (PP1-dep dephosphorylation rates)
    fp Kmpp1; // [uM]
    fp kib2;
    fp kb2i;
    fp kb24;
    fp kb42;
    fp kta; // [s^-1] (Ca4CaM dissociation from Wt)
    fp kat; // [uM^-1 s^-1] (Ca4CaM reassociation with Wa)
    fp kt42;
    fp kt24;
    fp kat2;
    fp kt2a;

    // CaN parameters
    fp kcanCaoff;   // [s^-1]
    fp kcanCaon;    // [uM^-1 s^-1]
    fp kcanCaM4on;  // [uM^-1 s^-1]
    fp kcanCaM4off; // [s^-1]
    fp kcanCaM2on;
    fp kcanCaM2off;
    fp kcanCaM0on;
    fp kcanCaM0off;
    fp k02can;
    fp k20can;
    fp k24can;
    fp k42can;

    // CaM Reaction fluxes
    fp rcn02;
    fp rcn24;

    // CaM buffer fluxes
    fp B;
    fp rcn02B;
    fp rcn24B;
    fp rcn0B;
    fp rcn2B;
    fp rcn4B;

    // CaN reaction fluxes
    fp Ca2CaN;
    fp rcnCa4CaN;
    fp rcn02CaN;
    fp rcn24CaN;
    fp rcn0CaN;
    fp rcn2CaN;
    fp rcn4CaN;

    // CaMKII reaction fluxes
    fp Pix;
    fp rcnCKib2;
    fp rcnCKb2b;
    fp rcnCKib;
    fp T;
    fp kbt;
    fp rcnCKbt;
    fp rcnCKtt2;
    fp rcnCKta;
    fp rcnCKt2a;
    fp rcnCKt2b2;
    fp rcnCKai;

    // CaM equations
    fp dCaM;
    fp dCa2CaM;
    fp dCa4CaM;
    fp dCaMB;
    fp dCa2CaMB;
    fp dCa4CaMB;

    // CaMKII equations
    fp dPb2; // Pb2
    fp dPb;  // Pb
    fp dPt;  // Pt
    fp dPt2; // Pt2
    fp dPa;  // Pa

    // CaN equations
    fp dCa4CaN;       // Ca4CaN
    fp dCaMCa4CaN;    // CaMCa4CaN
    fp dCa2CaMCa4CaN; // Ca2CaMCa4CaN
    fp dCa4CaMCa4CaN; // Ca4CaMCa4CaN

    //=====================================================================
    //	COMPUTATION
    //=====================================================================

    // input data and output data variable references
    offset_1 = initvalu_offset;
    offset_2 = initvalu_offset + 1;
    offset_3 = initvalu_offset + 2;
    offset_4 = initvalu_offset + 3;
    offset_5 = initvalu_offset + 4;
    offset_6 = initvalu_offset + 5;
    offset_7 = initvalu_offset + 6;
    offset_8 = initvalu_offset + 7;
    offset_9 = initvalu_offset + 8;
    offset_10 = initvalu_offset + 9;
    offset_11 = initvalu_offset + 10;
    offset_12 = initvalu_offset + 11;
    offset_13 = initvalu_offset + 12;
    offset_14 = initvalu_offset + 13;
    offset_15 = initvalu_offset + 14;

    // input parameters variable references
    parameter_offset_1 = parameter_offset;
    parameter_offset_2 = parameter_offset + 1;
    parameter_offset_3 = parameter_offset + 2;
    parameter_offset_4 = parameter_offset + 3;
    parameter_offset_5 = parameter_offset + 4;

    // decoding input array
    CaM = initvalu[offset_1];
    Ca2CaM = initvalu[offset_2];
    Ca4CaM = initvalu[offset_3];
    CaMB = initvalu[offset_4];
    Ca2CaMB = initvalu[offset_5];
    Ca4CaMB = initvalu[offset_6];
    Pb2 = initvalu[offset_7];
    Pb = initvalu[offset_8];
    Pt = initvalu[offset_9];
    Pt2 = initvalu[offset_10];
    Pa = initvalu[offset_11];
    Ca4CaN = initvalu[offset_12];
    CaMCa4CaN = initvalu[offset_13];
    Ca2CaMCa4CaN = initvalu[offset_14];
    Ca4CaMCa4CaN = initvalu[offset_15];

    // decoding input parameters
    CaMtot = parameter[parameter_offset_1];
    Btot = parameter[parameter_offset_2];
    CaMKIItot = parameter[parameter_offset_3];
    CaNtot = parameter[parameter_offset_4];
    PP1tot = parameter[parameter_offset_5];

    // values [CONSTANTS FOR ALL THREADS]
    K = 135; //
    Mg = 1;  //

    // Ca/CaM parameters
    if (Mg <= 1)
    {
        Kd02 = 0.0025 * (1 + K / 0.94 - Mg / 0.012) * (1 + K / 8.1 + Mg / 0.022);  // [uM^2]
        Kd24 = 0.128 * (1 + K / 0.64 + Mg / 0.0014) * (1 + K / 13.0 - Mg / 0.153); // [uM^2]
    }
    else
    {
        Kd02 = 0.0025 * (1 + K / 0.94 - 1 / 0.012 + (Mg - 1) / 0.060) * (1 + K / 8.1 + 1 / 0.022 + (Mg - 1) / 0.068);  // [uM^2]
        Kd24 = 0.128 * (1 + K / 0.64 + 1 / 0.0014 + (Mg - 1) / 0.005) * (1 + K / 13.0 - 1 / 0.153 + (Mg - 1) / 0.150); // [uM^2]
    }
    k20 = 10;         // [s^-1]
    k02 = k20 / Kd02; // [uM^-2 s^-1]
    k42 = 500;        // [s^-1]
    k24 = k42 / Kd24; // [uM^-2 s^-1]

    // CaM buffering (B) parameters
    k0Boff = 0.0014;       // [s^-1]
    k0Bon = k0Boff / 0.2;  // [uM^-1 s^-1] kon = koff/Kd
    k2Boff = k0Boff / 100; // [s^-1]
    k2Bon = k0Bon;         // [uM^-1 s^-1]
    k4Boff = k2Boff;       // [s^-1]
    k4Bon = k0Bon;         // [uM^-1 s^-1]

    // using thermodynamic constraints
    k20B = k20 / 100; // [s^-1] thermo constraint on loop 1
    k02B = k02;       // [uM^-2 s^-1]
    k42B = k42;       // [s^-1] thermo constraint on loop 2
    k24B = k24;       // [uM^-2 s^-1]

    // Wi Wa Wt Wp
    kbi = 2.2;           // [s^-1] (Ca4CaM dissocation from Wb)
    kib = kbi / 33.5e-3; // [uM^-1 s^-1]
    kpp1 = 1.72;         // [s^-1] (PP1-dep dephosphorylation rates)
    Kmpp1 = 11.5;        // [uM]
    kib2 = kib;
    kb2i = kib2 * 5;
    kb24 = k24;
    kb42 = k42 * 33.5e-3 / 5;
    kta = kbi / 1000; // [s^-1] (Ca4CaM dissociation from Wt)
    kat = kib;        // [uM^-1 s^-1] (Ca4CaM reassociation with Wa)
    kt42 = k42 * 33.5e-6 / 5;
    kt24 = k24;
    kat2 = kib;
    kt2a = kib * 5;

    // CaN parameters
    kcanCaoff = 1;              // [s^-1]
    kcanCaon = kcanCaoff / 0.5; // [uM^-1 s^-1]
    kcanCaM4on = 46;            // [uM^-1 s^-1]
    kcanCaM4off = 0.0013;       // [s^-1]
    kcanCaM2on = kcanCaM4on;
    kcanCaM2off = 2508 * kcanCaM4off;
    kcanCaM0on = kcanCaM4on;
    kcanCaM0off = 165 * kcanCaM2off;
    k02can = k02;
    k20can = k20 / 165;
    k24can = k24;
    k42can = k20 / 2508;

    // CaM Reaction fluxes
    rcn02 = k02 * pow(Ca, 2) * CaM - k20 * Ca2CaM;
    rcn24 = k24 * pow(Ca, 2) * Ca2CaM - k42 * Ca4CaM;

    // CaM buffer fluxes
    B = Btot - CaMB - Ca2CaMB - Ca4CaMB;
    rcn02B = k02B * pow(Ca, 2) * CaMB - k20B * Ca2CaMB;
    rcn24B = k24B * pow(Ca, 2) * Ca2CaMB - k42B * Ca4CaMB;
    rcn0B = k0Bon * CaM * B - k0Boff * CaMB;
    rcn2B = k2Bon * Ca2CaM * B - k2Boff * Ca2CaMB;
    rcn4B = k4Bon * Ca4CaM * B - k4Boff * Ca4CaMB;

    // CaN reaction fluxes
    Ca2CaN = CaNtot - Ca4CaN - CaMCa4CaN - Ca2CaMCa4CaN - Ca4CaMCa4CaN;
    rcnCa4CaN = kcanCaon * pow(Ca, 2) * Ca2CaN - kcanCaoff * Ca4CaN;
    rcn02CaN = k02can * pow(Ca, 2) * CaMCa4CaN - k20can * Ca2CaMCa4CaN;
    rcn24CaN = k24can * pow(Ca, 2) * Ca2CaMCa4CaN - k42can * Ca4CaMCa4CaN;
    rcn0CaN = kcanCaM0on * CaM * Ca4CaN - kcanCaM0off * CaMCa4CaN;
    rcn2CaN = kcanCaM2on * Ca2CaM * Ca4CaN - kcanCaM2off * Ca2CaMCa4CaN;
    rcn4CaN = kcanCaM4on * Ca4CaM * Ca4CaN - kcanCaM4off * Ca4CaMCa4CaN;

    // CaMKII reaction fluxes
    Pix = 1 - Pb2 - Pb - Pt - Pt2 - Pa;
    rcnCKib2 = kib2 * Ca2CaM * Pix - kb2i * Pb2;
    rcnCKb2b = kb24 * pow(Ca, 2) * Pb2 - kb42 * Pb;
    rcnCKib = kib * Ca4CaM * Pix - kbi * Pb;
    T = Pb + Pt + Pt2 + Pa;
    kbt = 0.055 * T + 0.0074 * pow(T, 2) + 0.015 * pow(T, 3);
    rcnCKbt = kbt * Pb - kpp1 * PP1tot * Pt / (Kmpp1 + CaMKIItot * Pt);
    rcnCKtt2 = kt42 * Pt - kt24 * pow(Ca, 2) * Pt2;
    rcnCKta = kta * Pt - kat * Ca4CaM * Pa;
    rcnCKt2a = kt2a * Pt2 - kat2 * Ca2CaM * Pa;
    rcnCKt2b2 = kpp1 * PP1tot * Pt2 / (Kmpp1 + CaMKIItot * Pt2);
    rcnCKai = kpp1 * PP1tot * Pa / (Kmpp1 + CaMKIItot * Pa);

    // CaM equations
    dCaM = 1e-3 * (-rcn02 - rcn0B - rcn0CaN);
    dCa2CaM = 1e-3 * (rcn02 - rcn24 - rcn2B - rcn2CaN + CaMKIItot * (-rcnCKib2 + rcnCKt2a));
    dCa4CaM = 1e-3 * (rcn24 - rcn4B - rcn4CaN + CaMKIItot * (-rcnCKib + rcnCKta));
    dCaMB = 1e-3 * (rcn0B - rcn02B);
    dCa2CaMB = 1e-3 * (rcn02B + rcn2B - rcn24B);
    dCa4CaMB = 1e-3 * (rcn24B + rcn4B);

    // CaMKII equations
    dPb2 = 1e-3 * (rcnCKib2 - rcnCKb2b + rcnCKt2b2); // Pb2
    dPb = 1e-3 * (rcnCKib + rcnCKb2b - rcnCKbt);     // Pb
    dPt = 1e-3 * (rcnCKbt - rcnCKta - rcnCKtt2);     // Pt
    dPt2 = 1e-3 * (rcnCKtt2 - rcnCKt2a - rcnCKt2b2); // Pt2
    dPa = 1e-3 * (rcnCKta + rcnCKt2a - rcnCKai);     // Pa

    // CaN equations
    dCa4CaN = 1e-3 * (rcnCa4CaN - rcn0CaN - rcn2CaN - rcn4CaN); // Ca4CaN
    dCaMCa4CaN = 1e-3 * (rcn0CaN - rcn02CaN);                   // CaMCa4CaN
    dCa2CaMCa4CaN = 1e-3 * (rcn2CaN + rcn02CaN - rcn24CaN);     // Ca2CaMCa4CaN
    dCa4CaMCa4CaN = 1e-3 * (rcn4CaN + rcn24CaN);                // Ca4CaMCa4CaN

    // encode output array
    finavalu[offset_1] = dCaM;
    finavalu[offset_2] = dCa2CaM;
    finavalu[offset_3] = dCa4CaM;
    finavalu[offset_4] = dCaMB;
    finavalu[offset_5] = dCa2CaMB;
    finavalu[offset_6] = dCa4CaMB;
    finavalu[offset_7] = dPb2;
    finavalu[offset_8] = dPb;
    finavalu[offset_9] = dPt;
    finavalu[offset_10] = dPt2;
    finavalu[offset_11] = dPa;
    finavalu[offset_12] = dCa4CaN;
    finavalu[offset_13] = dCaMCa4CaN;
    finavalu[offset_14] = dCa2CaMCa4CaN;
    finavalu[offset_15] = dCa4CaMCa4CaN;

    // write to global variables for adjusting Ca buffering in EC coupling model
    JCa = 1e-3 * (2 * CaMKIItot * (rcnCKtt2 - rcnCKb2b) - 2 * (rcn02 + rcn24 + rcn02B + rcn24B + rcnCa4CaN + rcn02CaN + rcn24CaN)); // [uM/msec]

    // return
    return JCa;
}

void fin(fp *initvalu,
         int initvalu_offset_ecc,
         int initvalu_offset_Dyad,
         int initvalu_offset_SL,
         int initvalu_offset_Cyt,
         fp *parameter,
         fp *finavalu,
         fp JCaDyad,
         fp JCaSL,
         fp JCaCyt)
{

    //=====================================================================
    //	VARIABLES
    //=====================================================================

    // decoded input parameters
    fp BtotDyad;      //
    fp CaMKIItotDyad; //

    // compute variables
    fp Vmyo;    // [L]
    fp Vdyad;   // [L]
    fp VSL;     // [L]
    fp kDyadSL; // [L/msec]
    fp kSLmyo;  // [L/msec]
    fp k0Boff;  // [s^-1]
    fp k0Bon;   // [uM^-1 s^-1] kon = koff/Kd
    fp k2Boff;  // [s^-1]
    fp k2Bon;   // [uM^-1 s^-1]
    fp k4Boff;  // [s^-1]
    fp k4Bon;   // [uM^-1 s^-1]
    fp CaMtotDyad;
    fp Bdyad;           // [uM dyad]
    fp J_cam_dyadSL;    // [uM/msec dyad]
    fp J_ca2cam_dyadSL; // [uM/msec dyad]
    fp J_ca4cam_dyadSL; // [uM/msec dyad]
    fp J_cam_SLmyo;     // [umol/msec]
    fp J_ca2cam_SLmyo;  // [umol/msec]
    fp J_ca4cam_SLmyo;  // [umol/msec]

    //=====================================================================
    //	COMPUTATION
    //=====================================================================

    // decoded input parameters
    BtotDyad = parameter[2];      //
    CaMKIItotDyad = parameter[3]; //

    // set variables
    Vmyo = 2.1454e-11;     // [L]
    Vdyad = 1.7790e-14;    // [L]
    VSL = 6.6013e-13;      // [L]
    kDyadSL = 3.6363e-16;  // [L/msec]
    kSLmyo = 8.587e-15;    // [L/msec]
    k0Boff = 0.0014;       // [s^-1]
    k0Bon = k0Boff / 0.2;  // [uM^-1 s^-1] kon = koff/Kd
    k2Boff = k0Boff / 100; // [s^-1]
    k2Bon = k0Bon;         // [uM^-1 s^-1]
    k4Boff = k2Boff;       // [s^-1]
    k4Bon = k0Bon;         // [uM^-1 s^-1]

    // ADJUST ECC incorporate Ca buffering from CaM, convert JCaCyt from uM/msec to mM/msec
    finavalu[initvalu_offset_ecc + 35] = finavalu[initvalu_offset_ecc + 35] + 1e-3 * JCaDyad;
    finavalu[initvalu_offset_ecc + 36] = finavalu[initvalu_offset_ecc + 36] + 1e-3 * JCaSL;
    finavalu[initvalu_offset_ecc + 37] = finavalu[initvalu_offset_ecc + 37] + 1e-3 * JCaCyt;

    // incorporate CaM diffusion between compartments
    CaMtotDyad = initvalu[initvalu_offset_Dyad + 0] + initvalu[initvalu_offset_Dyad + 1] + initvalu[initvalu_offset_Dyad + 2] + initvalu[initvalu_offset_Dyad + 3] + initvalu[initvalu_offset_Dyad + 4] + initvalu[initvalu_offset_Dyad + 5] + CaMKIItotDyad * (initvalu[initvalu_offset_Dyad + 6] + initvalu[initvalu_offset_Dyad + 7] + initvalu[initvalu_offset_Dyad + 8] + initvalu[initvalu_offset_Dyad + 9]) + initvalu[initvalu_offset_Dyad + 12] + initvalu[initvalu_offset_Dyad + 13] + initvalu[initvalu_offset_Dyad + 14];
    Bdyad = BtotDyad - CaMtotDyad;                                                                                             // [uM dyad]
    J_cam_dyadSL = 1e-3 * (k0Boff * initvalu[initvalu_offset_Dyad + 0] - k0Bon * Bdyad * initvalu[initvalu_offset_SL + 0]);    // [uM/msec dyad]
    J_ca2cam_dyadSL = 1e-3 * (k2Boff * initvalu[initvalu_offset_Dyad + 1] - k2Bon * Bdyad * initvalu[initvalu_offset_SL + 1]); // [uM/msec dyad]
    J_ca4cam_dyadSL = 1e-3 * (k2Boff * initvalu[initvalu_offset_Dyad + 2] - k4Bon * Bdyad * initvalu[initvalu_offset_SL + 2]); // [uM/msec dyad]

    J_cam_SLmyo = kSLmyo * (initvalu[initvalu_offset_SL + 0] - initvalu[initvalu_offset_Cyt + 0]);    // [umol/msec]
    J_ca2cam_SLmyo = kSLmyo * (initvalu[initvalu_offset_SL + 1] - initvalu[initvalu_offset_Cyt + 1]); // [umol/msec]
    J_ca4cam_SLmyo = kSLmyo * (initvalu[initvalu_offset_SL + 2] - initvalu[initvalu_offset_Cyt + 2]); // [umol/msec]

    // ADJUST CAM Dyad
    finavalu[initvalu_offset_Dyad + 0] = finavalu[initvalu_offset_Dyad + 0] - J_cam_dyadSL;
    finavalu[initvalu_offset_Dyad + 1] = finavalu[initvalu_offset_Dyad + 1] - J_ca2cam_dyadSL;
    finavalu[initvalu_offset_Dyad + 2] = finavalu[initvalu_offset_Dyad + 2] - J_ca4cam_dyadSL;

    // ADJUST CAM Sl
    finavalu[initvalu_offset_SL + 0] = finavalu[initvalu_offset_SL + 0] + J_cam_dyadSL * Vdyad / VSL - J_cam_SLmyo / VSL;
    finavalu[initvalu_offset_SL + 1] = finavalu[initvalu_offset_SL + 1] + J_ca2cam_dyadSL * Vdyad / VSL - J_ca2cam_SLmyo / VSL;
    finavalu[initvalu_offset_SL + 2] = finavalu[initvalu_offset_SL + 2] + J_ca4cam_dyadSL * Vdyad / VSL - J_ca4cam_SLmyo / VSL;

    // ADJUST CAM Cyt
    finavalu[initvalu_offset_Cyt + 0] = finavalu[initvalu_offset_Cyt + 0] + J_cam_SLmyo / Vmyo;
    finavalu[initvalu_offset_Cyt + 1] = finavalu[initvalu_offset_Cyt + 1] + J_ca2cam_SLmyo / Vmyo;
    finavalu[initvalu_offset_Cyt + 2] = finavalu[initvalu_offset_Cyt + 2] + J_ca4cam_SLmyo / Vmyo;
}

void master(fp timeinst,
            fp *initvalu,
            fp *parameter,
            fp *finavalu,
            int mode)
{

    //=====================================================================
    //	VARIABLES
    //=====================================================================

    // counters
    int i;

    // intermediate output on host
    fp JCaDyad;
    fp JCaSL;
    fp JCaCyt;

    // offset pointers
    int initvalu_offset_batch; //
    int initvalu_offset_ecc;   // 46 points
    int parameter_offset_ecc;
    int initvalu_offset_Dyad; // 15 points
    int parameter_offset_Dyad;
    int initvalu_offset_SL; // 15 points
    int parameter_offset_SL;
    int initvalu_offset_Cyt; // 15 poitns
    int parameter_offset_Cyt;

    // module parameters
    fp CaDyad; // from ECC model, *** Converting from [mM] to [uM] ***
    fp CaSL;   // from ECC model, *** Converting from [mM] to [uM] ***
    fp CaCyt;  // from ECC model, *** Converting from [mM] to [uM] ***

    // thread counters
    int th_id, nthreads;
    int th_count[4];
    int temp;

    //=====================================================================
    //	KERNELS FOR 1 WORKLOAD - PARALLEL
    //=====================================================================

    nthreads = 0; // omp_get_max_threads();

    //=====================================================================
    //	KERNELS FOR MANY WORKLOAD - SERIAL
    //=====================================================================

    // ecc function
    initvalu_offset_ecc = 0; // 46 points
    parameter_offset_ecc = 0;
    ecc(timeinst,
        initvalu,
        initvalu_offset_ecc,
        parameter,
        parameter_offset_ecc,
        finavalu);

    // cam function for Dyad
    initvalu_offset_Dyad = 46; // 15 points
    parameter_offset_Dyad = 1;
    CaDyad = initvalu[35] * 1e3; // from ECC model, *** Converting from [mM] to [uM] ***
    JCaDyad = cam(timeinst,
                  initvalu,
                  initvalu_offset_Dyad,
                  parameter,
                  parameter_offset_Dyad,
                  finavalu,
                  CaDyad);

    // cam function for SL
    initvalu_offset_SL = 61; // 15 points
    parameter_offset_SL = 6;
    CaSL = initvalu[36] * 1e3; // from ECC model, *** Converting from [mM] to [uM] ***
    JCaSL = cam(timeinst,
                initvalu,
                initvalu_offset_SL,
                parameter,
                parameter_offset_SL,
                finavalu,
                CaSL);

    // cam function for Cyt
    initvalu_offset_Cyt = 76; // 15 poitns
    parameter_offset_Cyt = 11;
    CaCyt = initvalu[37] * 1e3; // from ECC model, *** Converting from [mM] to [uM] ***
    JCaCyt = cam(timeinst,
                 initvalu,
                 initvalu_offset_Cyt,
                 parameter,
                 parameter_offset_Cyt,
                 finavalu,
                 CaCyt);

    //=====================================================================
    //	FINAL KERNEL
    //=====================================================================

    // final adjustments
    fin(initvalu,
        initvalu_offset_ecc,
        initvalu_offset_Dyad,
        initvalu_offset_SL,
        initvalu_offset_Cyt,
        parameter,
        finavalu,
        JCaDyad,
        JCaSL,
        JCaCyt);

    //=====================================================================
    //	COMPENSATION FOR NANs and INFs
    //=====================================================================

    // make sure function does not return NANs and INFs
    for (i = 0; i < EQUATIONS; i++)
    {
        if (isnan(finavalu[i]) == 1)
        {
            finavalu[i] = 0.0001; // for NAN set rate of change to 0.0001
        }
        else if (isinf(finavalu[i]) == 1)
        {
            finavalu[i] = 0.0001; // for INF set rate of change to 0.0001
        }
    }
}

static fp embedded_fehlberg_7_8(fp timeinst,
                                fp h,
                                fp *initvalu,
                                fp *finavalu,
                                fp *error,
                                fp *parameter,
                                int mode)
{

    // printf("initvalu[0] = %f\n", initvalu[0]);
    // printf("initvalu[10] = %f\n", initvalu[10]);
    // printf("initvalu[50] = %f\n", initvalu[50]);
    // printf("initvalu[90] = %f\n", initvalu[90]);

    // printf("finavalu[0] = %f\n", finavalu[0]);
    // printf("finavalu[10] = %f\n", finavalu[10]);
    // printf("finavalu[50] = %f\n", finavalu[50]);
    // printf("finavalu[90] = %f\n", finavalu[90]);

    //======================================================================================================================================================
    //	VARIABLES
    //======================================================================================================================================================

    static const fp c_1_11 = 41.0 / 840.0;
    static const fp c6 = 34.0 / 105.0;
    static const fp c_7_8 = 9.0 / 35.0;
    static const fp c_9_10 = 9.0 / 280.0;

    static const fp a2 = 2.0 / 27.0;
    static const fp a3 = 1.0 / 9.0;
    static const fp a4 = 1.0 / 6.0;
    static const fp a5 = 5.0 / 12.0;
    static const fp a6 = 1.0 / 2.0;
    static const fp a7 = 5.0 / 6.0;
    static const fp a8 = 1.0 / 6.0;
    static const fp a9 = 2.0 / 3.0;
    static const fp a10 = 1.0 / 3.0;

    static const fp b31 = 1.0 / 36.0;
    static const fp b32 = 3.0 / 36.0;
    static const fp b41 = 1.0 / 24.0;
    static const fp b43 = 3.0 / 24.0;
    static const fp b51 = 20.0 / 48.0;
    static const fp b53 = -75.0 / 48.0;
    static const fp b54 = 75.0 / 48.0;
    static const fp b61 = 1.0 / 20.0;
    static const fp b64 = 5.0 / 20.0;
    static const fp b65 = 4.0 / 20.0;
    static const fp b71 = -25.0 / 108.0;
    static const fp b74 = 125.0 / 108.0;
    static const fp b75 = -260.0 / 108.0;
    static const fp b76 = 250.0 / 108.0;
    static const fp b81 = 31.0 / 300.0;
    static const fp b85 = 61.0 / 225.0;
    static const fp b86 = -2.0 / 9.0;
    static const fp b87 = 13.0 / 900.0;
    static const fp b91 = 2.0;
    static const fp b94 = -53.0 / 6.0;
    static const fp b95 = 704.0 / 45.0;
    static const fp b96 = -107.0 / 9.0;
    static const fp b97 = 67.0 / 90.0;
    static const fp b98 = 3.0;
    static const fp b10_1 = -91.0 / 108.0;
    static const fp b10_4 = 23.0 / 108.0;
    static const fp b10_5 = -976.0 / 135.0;
    static const fp b10_6 = 311.0 / 54.0;
    static const fp b10_7 = -19.0 / 60.0;
    static const fp b10_8 = 17.0 / 6.0;
    static const fp b10_9 = -1.0 / 12.0;
    static const fp b11_1 = 2383.0 / 4100.0;
    static const fp b11_4 = -341.0 / 164.0;
    static const fp b11_5 = 4496.0 / 1025.0;
    static const fp b11_6 = -301.0 / 82.0;
    static const fp b11_7 = 2133.0 / 4100.0;
    static const fp b11_8 = 45.0 / 82.0;
    static const fp b11_9 = 45.0 / 164.0;
    static const fp b11_10 = 18.0 / 41.0;
    static const fp b12_1 = 3.0 / 205.0;
    static const fp b12_6 = -6.0 / 41.0;
    static const fp b12_7 = -3.0 / 205.0;
    static const fp b12_8 = -3.0 / 41.0;
    static const fp b12_9 = 3.0 / 41.0;
    static const fp b12_10 = 6.0 / 41.0;
    static const fp b13_1 = -1777.0 / 4100.0;
    static const fp b13_4 = -341.0 / 164.0;
    static const fp b13_5 = 4496.0 / 1025.0;
    static const fp b13_6 = -289.0 / 82.0;
    static const fp b13_7 = 2193.0 / 4100.0;
    static const fp b13_8 = 51.0 / 82.0;
    static const fp b13_9 = 33.0 / 164.0;
    static const fp b13_10 = 12.0 / 41.0;

    static const fp err_factor = -41.0 / 840.0;

    fp h2_7 = a2 * h;

    fp timeinst_temp;
    fp *initvalu_temp;
    fp **finavalu_temp;

    int i;

    //======================================================================================================================================================
    //		TEMPORARY STORAGE ALLOCATION
    //======================================================================================================================================================

    initvalu_temp = (fp *)malloc(EQUATIONS * sizeof(fp));

    finavalu_temp = (fp **)malloc(13 * sizeof(fp *));
    for (i = 0; i < 13; i++)
    {
        finavalu_temp[i] = (fp *)malloc(EQUATIONS * sizeof(fp));
    }

    //======================================================================================================================================================
    //		EVALUATIONS
    //======================================================================================================================================================

    //===================================================================================================
    //		1
    //===================================================================================================

    timeinst_temp = timeinst;
    for (i = 0; i < EQUATIONS; i++)
    {
        initvalu_temp[i] = initvalu[i];
        // printf("initvalu[%d] = %f\n", i, initvalu[i]);
    }

    master(timeinst_temp,
           initvalu_temp,
           parameter,
           finavalu_temp[0],
           mode);

    // for(i=0; i<EQUATIONS; i++){
    // printf("finavalu_temp[0][%d] = %f\n", i, finavalu_temp[0][i]);
    // }

    //===================================================================================================
    //		2
    //===================================================================================================

    timeinst_temp = timeinst + h2_7;
    for (i = 0; i < EQUATIONS; i++)
    {
        initvalu_temp[i] = initvalu[i] + h2_7 * (finavalu_temp[0][i]);
    }

    master(timeinst_temp,
           initvalu_temp,
           parameter,
           finavalu_temp[1],
           mode);

    // for(i=0; i<EQUATIONS; i++){
    // printf("finavalu_temp[1][%d] = %f\n", i, finavalu_temp[1][i]);
    // }

    //===================================================================================================
    //		3
    //===================================================================================================

    timeinst_temp = timeinst + a3 * h;
    for (i = 0; i < EQUATIONS; i++)
    {
        initvalu_temp[i] = initvalu[i] + h * (b31 * finavalu_temp[0][i] + b32 * finavalu_temp[1][i]);
    }

    master(timeinst_temp,
           initvalu_temp,
           parameter,
           finavalu_temp[2],
           mode);

    // for(i=0; i<EQUATIONS; i++){
    // printf("finavalu_temp[2][%d] = %f\n", i, finavalu_temp[2][i]);
    // }

    //===================================================================================================
    //		4
    //===================================================================================================

    timeinst_temp = timeinst + a4 * h;
    for (i = 0; i < EQUATIONS; i++)
    {
        initvalu_temp[i] = initvalu[i] + h * (b41 * finavalu_temp[0][i] + b43 * finavalu_temp[2][i]);
    }

    master(timeinst_temp,
           initvalu_temp,
           parameter,
           finavalu_temp[3],
           mode);

    // for(i=0; i<EQUATIONS; i++){
    // printf("finavalu_temp[3][%d] = %f\n", i, finavalu_temp[3][i]);
    // }

    //===================================================================================================
    //		5
    //===================================================================================================

    timeinst_temp = timeinst + a5 * h;
    for (i = 0; i < EQUATIONS; i++)
    {
        initvalu_temp[i] = initvalu[i] + h * (b51 * finavalu_temp[0][i] + b53 * finavalu_temp[2][i] + b54 * finavalu_temp[3][i]);
    }

    master(timeinst_temp,
           initvalu_temp,
           parameter,
           finavalu_temp[4],
           mode);

    // for(i=0; i<EQUATIONS; i++){
    // printf("finavalu_temp[4][%d] = %f\n", i, finavalu_temp[4][i]);
    // }

    //===================================================================================================
    //		6
    //===================================================================================================

    timeinst_temp = timeinst + a6 * h;
    for (i = 0; i < EQUATIONS; i++)
    {
        initvalu_temp[i] = initvalu[i] + h * (b61 * finavalu_temp[0][i] + b64 * finavalu_temp[3][i] + b65 * finavalu_temp[4][i]);
    }

    master(timeinst_temp,
           initvalu_temp,
           parameter,
           finavalu_temp[5],
           mode);

    // for(i=0; i<EQUATIONS; i++){
    // printf("finavalu_temp[5][%d] = %f\n", i, finavalu_temp[5][i]);
    // }

    //===================================================================================================
    //		7
    //===================================================================================================

    timeinst_temp = timeinst + a7 * h;
    for (i = 0; i < EQUATIONS; i++)
    {
        initvalu_temp[i] = initvalu[i] + h * (b71 * finavalu_temp[0][i] + b74 * finavalu_temp[3][i] + b75 * finavalu_temp[4][i] + b76 * finavalu_temp[5][i]);
    }

    master(timeinst_temp,
           initvalu_temp,
           parameter,
           finavalu_temp[6],
           mode);

    // for(i=0; i<EQUATIONS; i++){
    // printf("finavalu_temp[6][%d] = %f\n", i, finavalu_temp[6][i]);
    // }

    //===================================================================================================
    //		8
    //===================================================================================================

    timeinst_temp = timeinst + a8 * h;
    for (i = 0; i < EQUATIONS; i++)
    {
        initvalu_temp[i] = initvalu[i] + h * (b81 * finavalu_temp[0][i] + b85 * finavalu_temp[4][i] + b86 * finavalu_temp[5][i] + b87 * finavalu_temp[6][i]);
    }

    master(timeinst_temp,
           initvalu_temp,
           parameter,
           finavalu_temp[7],
           mode);

    // for(i=0; i<EQUATIONS; i++){
    // printf("finavalu_temp[7][%d] = %f\n", i, finavalu_temp[7][i]);
    // }

    //===================================================================================================
    //		9
    //===================================================================================================

    timeinst_temp = timeinst + a9 * h;
    for (i = 0; i < EQUATIONS; i++)
    {
        initvalu_temp[i] = initvalu[i] + h * (b91 * finavalu_temp[0][i] + b94 * finavalu_temp[3][i] + b95 * finavalu_temp[4][i] + b96 * finavalu_temp[5][i] + b97 * finavalu_temp[6][i] + b98 * finavalu_temp[7][i]);
    }

    master(timeinst_temp,
           initvalu_temp,
           parameter,
           finavalu_temp[8],
           mode);

    // for(i=0; i<EQUATIONS; i++){
    // printf("finavalu_temp[8][%d] = %f\n", i, finavalu_temp[8][i]);
    // }

    //===================================================================================================
    //		10
    //===================================================================================================

    timeinst_temp = timeinst + a10 * h;
    for (i = 0; i < EQUATIONS; i++)
    {
        initvalu_temp[i] = initvalu[i] + h * (b10_1 * finavalu_temp[0][i] + b10_4 * finavalu_temp[3][i] + b10_5 * finavalu_temp[4][i] + b10_6 * finavalu_temp[5][i] + b10_7 * finavalu_temp[6][i] + b10_8 * finavalu_temp[7][i] + b10_9 * finavalu_temp[8][i]);
    }

    master(timeinst_temp,
           initvalu_temp,
           parameter,
           finavalu_temp[9],
           mode);

    // for(i=0; i<EQUATIONS; i++){
    // printf("finavalu_temp[9][%d] = %f\n", i, finavalu_temp[9][i]);
    // }

    //===================================================================================================
    //		11
    //===================================================================================================

    timeinst_temp = timeinst + h;
    for (i = 0; i < EQUATIONS; i++)
    {
        initvalu_temp[i] = initvalu[i] + h * (b11_1 * finavalu_temp[0][i] + b11_4 * finavalu_temp[3][i] + b11_5 * finavalu_temp[4][i] + b11_6 * finavalu_temp[5][i] + b11_7 * finavalu_temp[6][i] + b11_8 * finavalu_temp[7][i] + b11_9 * finavalu_temp[8][i] + b11_10 * finavalu_temp[9][i]);
    }

    master(timeinst_temp,
           initvalu_temp,
           parameter,
           finavalu_temp[10],
           mode);

    // for(i=0; i<EQUATIONS; i++){
    // printf("finavalu_temp[10][%d] = %f\n", i, finavalu_temp[10][i]);
    // }

    //===================================================================================================
    //		12
    //===================================================================================================

    timeinst_temp = timeinst;
    for (i = 0; i < EQUATIONS; i++)
    {
        initvalu_temp[i] = initvalu[i] + h * (b12_1 * finavalu_temp[0][i] + b12_6 * finavalu_temp[5][i] + b12_7 * finavalu_temp[6][i] + b12_8 * finavalu_temp[7][i] + b12_9 * finavalu_temp[8][i] + b12_10 * finavalu_temp[9][i]);
    }

    master(timeinst_temp,
           initvalu_temp,
           parameter,
           finavalu_temp[11],
           mode);

    // for(i=0; i<EQUATIONS; i++){
    // printf("finavalu_temp[11][%d] = %f\n", i, finavalu_temp[11][i]);
    // }

    //===================================================================================================
    //		13
    //===================================================================================================

    timeinst_temp = timeinst + h;
    for (i = 0; i < EQUATIONS; i++)
    {
        initvalu_temp[i] = initvalu[i] + h * (b13_1 * finavalu_temp[0][i] + b13_4 * finavalu_temp[3][i] + b13_5 * finavalu_temp[4][i] + b13_6 * finavalu_temp[5][i] + b13_7 * finavalu_temp[6][i] + b13_8 * finavalu_temp[7][i] + b13_9 * finavalu_temp[8][i] + b13_10 * finavalu_temp[9][i] + finavalu_temp[11][i]);
    }

    master(timeinst_temp,
           initvalu_temp,
           parameter,
           finavalu_temp[12],
           mode);

    // for(i=0; i<EQUATIONS; i++){
    // printf("finavalu_temp[12][%d] = %f\n", i, finavalu_temp[12][i]);
    // }

    //======================================================================================================================================================
    //		FINAL VALUE
    //======================================================================================================================================================

    for (i = 0; i < EQUATIONS; i++)
    {
        finavalu[i] = initvalu[i] + h * (c_1_11 * (finavalu_temp[0][i] + finavalu_temp[10][i]) + c6 * finavalu_temp[5][i] + c_7_8 * (finavalu_temp[6][i] + finavalu_temp[7][i]) + c_9_10 * (finavalu_temp[8][i] + finavalu_temp[9][i]));
        // printf("finavalu_temp[0][%d] = %f\n", i, finavalu_temp[0][i]);
        // printf("finavalu_temp[10][%d] = %f\n", i, finavalu_temp[10][i]);
        // printf("finavalu_temp[5][%d] = %f\n", i, finavalu_temp[5][i]);
        // printf("finavalu_temp[6][%d] = %f\n", i, finavalu_temp[6][i]);
        // printf("finavalu_temp[7][%d] = %f\n", i, finavalu_temp[7][i]);
        // printf("finavalu_temp[8][%d] = %f\n", i, finavalu_temp[8][i]);
        // printf("finavalu_temp[9][%d] = %f\n", i, finavalu_temp[9][i]);
        // printf("finavalu[%d] = %f\n", i, finavalu[i]);
    }

    //======================================================================================================================================================
    //		RETURN
    //======================================================================================================================================================

    for (i = 0; i < EQUATIONS; i++)
    {
        error[i] = fabs(err_factor * (finavalu_temp[0][i] + finavalu_temp[10][i] - finavalu_temp[11][i] - finavalu_temp[12][i]));
        // printf("Error[%d] = %f\n", i, error[i]);
    }

    //======================================================================================================================================================
    //		DEALLOCATION
    //======================================================================================================================================================

    free(initvalu_temp);
    free(finavalu_temp);
}

int solver(fp **y,
           fp *x,
           int xmax,
           fp *params,
           int mode)
{

    //========================================================================================================================
    //	VARIABLES
    //========================================================================================================================

    // solver parameters
    fp err_exponent;
    fp last_interval;
    int error;
    int outside;
    fp h;
    fp h_init;
    fp tolerance;
    int xmin;

    // memory
    fp scale_min;
    fp scale_fina;
    fp *err = (fp *)malloc(EQUATIONS * sizeof(fp));
    fp *scale = (fp *)malloc(EQUATIONS * sizeof(fp));
    fp *yy = (fp *)malloc(EQUATIONS * sizeof(fp));

    // counters
    int i, j, k;

    //========================================================================================================================
    //		INITIAL SETUP
    //========================================================================================================================

    // solver parameters
    err_exponent = 1.0 / 7.0;
    last_interval = 0;
    h_init = 1;
    h = h_init;
    xmin = 0;
    tolerance = 10 / (fp)(xmax - xmin);

    // save value for initial time instance
    x[0] = 0;

    //========================================================================================================================
    //		CHECKING
    //========================================================================================================================

    // Verify that the step size is positive and that the upper endpoint of integration is greater than the initial enpoint.               //
    if (xmax < xmin || h <= 0.0)
    {
        return -2;
    }

    // If the upper endpoint of the independent variable agrees with the initial value of the independent variable.  Set the value of the dependent variable and return success. //
    if (xmax == xmin)
    {
        return 0;
    }

    // Insure that the step size h is not larger than the length of the integration interval.                                            //
    if (h > (xmax - xmin))
    {
        h = (fp)xmax - (fp)xmin;
        last_interval = 1;
    }

    //========================================================================================================================
    //		SOLVING
    //========================================================================================================================

    for (k = 1; k <= xmax; k++)
    { // start after initial value

        x[k] = k - 1;
        h = h_init;

        //==========================================================================================
        //		REINITIALIZE VARIABLES
        //==========================================================================================

        scale_fina = 1.0;

        //==========================================================================================
        //		MAKE ATTEMPTS TO MINIMIZE ERROR
        //==========================================================================================

        // make attempts to minimize error
        for (j = 0; j < ATTEMPTS; j++)
        {

            //============================================================
            //		REINITIALIZE VARIABLES
            //============================================================

            error = 0;
            outside = 0;
            scale_min = MAX_SCALE_FACTOR;

            //============================================================
            //		EVALUATE ALL EQUATIONS
            //============================================================

            embedded_fehlberg_7_8(x[k],
                                  h,
                                  y[k - 1],
                                  y[k],
                                  err,
                                  params,
                                  mode);

            //============================================================
            //		IF THERE WAS NO ERROR FOR ANY OF EQUATIONS, SET SCALE AND LEAVE THE LOOP
            //============================================================

            for (i = 0; i < EQUATIONS; i++)
            {
                if (err[i] > 0)
                {
                    error = 1;
                }
            }
            if (error != 1)
            {
                scale_fina = MAX_SCALE_FACTOR;
                break;
            }

            //============================================================
            //		FIGURE OUT SCALE AS THE MINIMUM OF COMPONENT SCALES
            //============================================================

            for (i = 0; i < EQUATIONS; i++)
            {
                if (y[k - 1][i] == 0.0)
                {
                    yy[i] = tolerance;
                }
                else
                {
                    yy[i] = fabs(y[k - 1][i]);
                }
                scale[i] = 0.8 * pow(tolerance * yy[i] / err[i], err_exponent);
                if (scale[i] < scale_min)
                {
                    scale_min = scale[i];
                }
            }
            scale_fina = min(max(scale_min, MIN_SCALE_FACTOR), MAX_SCALE_FACTOR);

            //============================================================
            //		IF WITHIN TOLERANCE, FINISH ATTEMPTS...
            //============================================================

            for (i = 0; i < EQUATIONS; i++)
            {
                if (err[i] > (tolerance * yy[i]))
                {
                    outside = 1;
                }
            }
            if (outside == 0)
            {
                break;
            }

            //============================================================
            //		...OTHERWISE, ADJUST STEP FOR NEXT ATTEMPT
            //============================================================

            // scale next step in a default way
            h = h * scale_fina;

            // limit step to 0.9, because when it gets close to 1, it no longer makes sense, as 1 is already the next time instance (added to original algorithm)
            if (h >= 0.9)
            {
                h = 0.9;
            }

            // if instance+step exceeds range limit, limit to that range
            if (x[k] + h > (fp)xmax)
            {
                h = (fp)xmax - x[k];
            }

            // if getting closer to range limit, decrease step
            else if (x[k] + h + 0.5 * h > (fp)xmax)
            {
                h = 0.5 * h;
            }
        }

        //==========================================================================================
        //		SAVE TIME INSTANCE THAT SOLVER ENDED UP USING
        //==========================================================================================

        x[k] = x[k] + h;

        //==========================================================================================
        //		IF MAXIMUM NUMBER OF ATTEMPTS REACHED AND CANNOT GIVE SOLUTION, EXIT PROGRAM WITH ERROR
        //==========================================================================================

        if (j >= ATTEMPTS)
        {
            return -1;
        }
    }

    //========================================================================================================================
    //		FREE MEMORY
    //========================================================================================================================

    free(err);
    free(scale);
    free(yy);

    //========================================================================================================================
    //		FINAL RETURN
    //========================================================================================================================

    return 0;

    //======================================================================================================================================================
    //======================================================================================================================================================
    //		END OF SOLVER FUNCTION
    //======================================================================================================================================================
    //======================================================================================================================================================
}

void write(char *filename,
           fp *input,
           int data_rows,
           int data_cols,
           int major,
           int data_range)
{

    //=====================================================================
    //	VARIABLES
    //=====================================================================

    FILE *fid;
    int i, j;

    //=====================================================================
    //	CREATE/OPEN FILE FOR WRITING
    //=====================================================================

    fid = fopen(filename, "w");
    if (fid == NULL)
    {
        printf("The file was not created/opened for writing\n");
        return;
    }

    //=====================================================================
    //	WRITE VALUES TO THE FILE
    //=====================================================================

    // if matrix is saved row major in memory (C)
    if (major == 0)
    {
        for (i = 0; i < data_rows; i++)
        {
            for (j = 0; j < data_cols; j++)
            {
                fprintf(fid, "%f ", (fp)input[i * data_cols + j]);
            }
            fprintf(fid, "\n");
        }
    }
    // if matrix is saved column major in memory (MATLAB)
    else
    {
        for (i = 0; i < data_rows; i++)
        {
            for (j = 0; j < data_cols; j++)
            {
                fprintf(fid, "%f ", (fp)input[j * data_rows + i]);
            }
            fprintf(fid, "\n");
        }
    }

    //=====================================================================
    //	CLOSE FILE
    //=====================================================================

    fclose(fid);
}

//========================================================================================================================
//	READ FUNCTION
//========================================================================================================================

void read(char *filename,
          fp *input,
          int data_rows,
          int data_cols,
          int major)
{

    //=====================================================================
    //	VARIABLES
    //=====================================================================

    FILE *fid;
    int i, j;
    char c;
    fp temp;

    //=====================================================================
    //	OPEN FILE FOR READING
    //=====================================================================

    fid = fopen(filename, "r");
    if (fid == NULL)
    {
        printf("The file was not opened for reading\n");
        return;
    }

    //=====================================================================
    //	READ VALUES FROM THE FILE
    //=====================================================================

    if (major == 0)
    { // if matrix is saved row major in memory (C)
        for (i = 0; i < data_rows; i++)
        {
            for (j = 0; j < data_cols; j++)
            {
                fscanf(fid, "%f", &temp);
                input[i * data_cols + j] = (fp)temp;
            }
        }
    }
    else
    { // if matrix is saved column major in memory (MATLAB)
        for (i = 0; i < data_rows; i++)
        {
            for (j = 0; j < data_cols; j++)
            {
                fscanf(fid, "%f", &temp);
                input[j * data_rows + i] = (fp)temp;
            }
        }
    }

    //=====================================================================
    //	CLOSE FILE
    //=====================================================================

    fclose(fid);
}

long long get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000000) + tv.tv_usec;
}

//====================================================================================================100
//	MAIN FUNCTION
//====================================================================================================100

int main(int argc, char *argv[])
{

    //================================================================================80
    //		VARIABLES
    //================================================================================80

    //============================================================60
    //		TIME
    //============================================================60

    long long time0;
    long long time1;
    long long time2;
    long long time3;
    long long time4;
    long long time5;

    time0 = get_time();

    //============================================================60
    //		COUNTERS
    //============================================================60

    long long memory;
    int i, j;
    int status;
    int mode;

    //============================================================60
    //		SOLVER PARAMETERS
    //============================================================60

    long workload;
    long xmin;
    long xmax;
    fp h;
    fp tolerance;

    //============================================================60
    //		DATA
    //============================================================60

    fp ***y;
    fp **x;
    fp **params;

    //============================================================60
    //		OPENMP
    //============================================================60

    int threads;

    //================================================================================80
    // 	GET INPUT PARAMETERS
    //================================================================================80

    //============================================================60
    //		CHECK NUMBER OF ARGUMENTS
    //============================================================60

    if (argc != 5)
    {
        printf("ERROR: %d is the incorrect number of arguments, the number of arguments must be 4\n", argc - 1);
        return 0;
    }

    //============================================================60
    //		GET AND CHECK PARTICULAR ARGUMENTS
    //============================================================60

    else
    {

        //========================================40
        //		SPAN
        //========================================40

        xmax = atoi(argv[1]);
        if (xmax < 0)
        {
            printf("ERROR: %d is the incorrect end of simulation interval, use numbers > 0\n", xmax);
            return 0;
        }

        //========================================40
        //		WORKLOAD
        //========================================40

        workload = atoi(argv[2]);
        if (workload < 0)
        {
            printf("ERROR: %d is the incorrect number of instances of simulation, use numbers > 0\n", workload);
            return 0;
        }

        //========================================40
        //		MODE
        //========================================40

        mode = 0;
        mode = atoi(argv[3]);
        if (mode != 0 && mode != 1)
        {
            printf("ERROR: %d is the incorrect mode, it should be omitted or equal to 0 or 1\n", mode);
            return 0;
        }

        //========================================40
        //		THREADS
        //========================================40

        threads = atoi(argv[4]);
        if (threads < 0)
        {
            printf("ERROR: %d is the incorrect number of threads, use numbers > 0\n", threads);
            return 0;
        }
        // omp_set_num_threads(threads);
    }

    time1 = get_time();

    //================================================================================80
    // 	ALLOCATE MEMORY
    //================================================================================80

    //============================================================60
    //		MEMORY CHECK
    //============================================================60

    memory = workload * (xmax + 1) * EQUATIONS * 4;
    if (memory > 1000000000)
    {
        printf("ERROR: trying to allocate more than 1.0GB of memory, decrease workload and span parameters or change memory parameter\n");
        return 0;
    }

    //============================================================60
    // 	ALLOCATE ARRAYS
    //============================================================60

    y = (fp ***)malloc(workload * sizeof(fp **));
    for (i = 0; i < workload; i++)
    {
        y[i] = (fp **)malloc((1 + xmax) * sizeof(fp *));
        for (j = 0; j < (1 + xmax); j++)
        {
            y[i][j] = (fp *)malloc(EQUATIONS * sizeof(fp));
        }
    }

    x = (fp **)malloc(workload * sizeof(fp *));
    for (i = 0; i < workload; i++)
    {
        x[i] = (fp *)malloc((1 + xmax) * sizeof(fp));
    }

    params = (fp **)malloc(workload * sizeof(fp *));
    for (i = 0; i < workload; i++)
    {
        params[i] = (fp *)malloc(PARAMETERS * sizeof(fp));
    }

    time2 = get_time();

    //================================================================================80
    // 	INITIAL VALUES
    //================================================================================80

    // y
    for (i = 0; i < workload; i++)
    {
        read("../../data/myocyte/y.txt",
             y[i][0],
             91,
             1,
             0);
    }

    // params
    for (i = 0; i < workload; i++)
    {
        read("../../data/myocyte/params.txt",
             params[i],
             16,
             1,
             0);
    }

    time3 = get_time();

    //================================================================================80
    //	EXECUTION
    //================================================================================80

    if (mode == 0)
    {

        for (i = 0; i < workload; i++)
        {

            status = solver(y[i],
                            x[i],
                            xmax,
                            params[i],
                            mode);

            // if(status !=0){
            // printf("STATUS: %d\n", status);
            // }
        }
    }
    else
    {

#pragma omp parallel for private(i, status) shared(y, x, xmax, params, mode)
        for (i = 0; i < workload; i++)
        {

            status = solver(y[i],
                            x[i],
                            xmax,
                            params[i],
                            mode);

            // if(status !=0){
            // printf("STATUS: %d\n", status);
            // }
        }
    }

    // // print results
    // int k;
    // for(i=0; i<workload; i++){
    // printf("WORKLOAD %d:\n", i);
    // for(j=0; j<(xmax+1); j++){
    // printf("\tTIME %d:\n", j);
    // for(k=0; k<EQUATIONS; k++){
    // printf("\t\ty[%d][%d][%d]=%13.10f\n", i, j, k, y[i][j][k]);
    // }
    // }
    // }

    time4 = get_time();

    //================================================================================80
    //	DEALLOCATION
    //================================================================================80

    // y values
    for (i = 0; i < workload; i++)
    {
        for (j = 0; j < (1 + xmax); j++)
        {
            free(y[i][j]);
        }
        free(y[i]);
    }
    free(y);

    // x values
    for (i = 0; i < workload; i++)
    {
        free(x[i]);
    }
    free(x);

    // parameters
    for (i = 0; i < workload; i++)
    {
        free(params[i]);
    }
    free(params);

    time5 = get_time();

    //================================================================================80
    //		DISPLAY TIMING
    //================================================================================80

    printf("Time spent in different stages of the application:\n");
    printf("%.12f s, %.12f % : SETUP VARIABLES, READ COMMAND LINE ARGUMENTS\n", (float)(time1 - time0) / 1000000, (float)(time1 - time0) / (float)(time5 - time0) * 100);
    printf("%.12f s, %.12f % : ALLOCATE MEMORY\n", (float)(time2 - time1) / 1000000, (float)(time2 - time1) / (float)(time5 - time0) * 100);
    printf("%.12f s, %.12f % : READ DATA FROM FILES\n", (float)(time3 - time2) / 1000000, (float)(time3 - time2) / (float)(time5 - time0) * 100);
    printf("%.12f s, %.12f % : RUN COMPUTATION\n", (float)(time4 - time3) / 1000000, (float)(time4 - time3) / (float)(time5 - time0) * 100);
    printf("%.12f s, %.12f % : FREE MEMORY\n", (float)(time5 - time4) / 1000000, (float)(time5 - time4) / (float)(time5 - time0) * 100);
    printf("Total time:\n");
    printf("%.12f s\n", (float)(time5 - time0) / 1000000);

    //====================================================================================================100
    //	END OF FILE
    //====================================================================================================100
}
