/*
 * Do not modify this file; it is automatically generated from the template
 * linkcmd.xdt in the ti.platforms.simplelink package and will be overwritten.
 */

/*
 * put '"'s around paths because, without this, the linker
 * considers '-' as minus operator, not a file name character.
 */


-l"/Users/mikael/Code/cc1350-swim-thermo/software/pinInterrupt_CC1350_SWIMTHERMO/Debug/configPkg/package/cfg/pinInterrupt_pem3.oem3"
-l"/Users/mikael/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/tidrivers_cc13xx_cc26xx_2_21_00_04/packages/ti/mw/display/lib/display.aem3"
-l"/Users/mikael/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/tidrivers_cc13xx_cc26xx_2_21_00_04/packages/ti/mw/lcd/lib/lcd.aem3"
-l"/Users/mikael/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/tidrivers_cc13xx_cc26xx_2_21_00_04/packages/ti/mw/grlib/lib/grlib.aem3"
-l"/Users/mikael/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/tidrivers_cc13xx_cc26xx_2_21_00_04/packages/ti/drivers/rf/lib/rf_singleMode_cc13xxware.aem3"
-l"/Users/mikael/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/tidrivers_cc13xx_cc26xx_2_21_00_04/packages/ti/drivers/lib/drivers_cc13xxware.aem3"
-l"/Users/mikael/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/tidrivers_cc13xx_cc26xx_2_21_00_04/packages/ti/drivers/lib/power_cc13xx_tirtos.aem3"
-l"/Users/mikael/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/tidrivers_cc13xx_cc26xx_2_21_00_04/packages/ti/drivers/pdm/lib/pdm_cc13xxware.aem3"
-l"/Users/mikael/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/tidrivers_cc13xx_cc26xx_2_21_00_04/packages/ti/mw/fatfs/lib/release/ti.mw.fatfs.aem3"
-l"/Users/mikael/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/tidrivers_cc13xx_cc26xx_2_21_00_04/packages/ti/drivers/ports/lib/tirtosport.aem3"
-l"/Users/mikael/Code/cc1350-swim-thermo/software/pinInterrupt_CC1350_SWIMTHERMO/src/sysbios/rom_sysbios.aem3"
-l"/Users/mikael/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/bios_6_46_01_37/packages/ti/targets/arm/rtsarm/lib/boot.aem3"
-l"/Users/mikael/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/bios_6_46_01_37/packages/ti/targets/arm/rtsarm/lib/auto_init.aem3"

--retain="*(xdc.meta)"

/* C6x Elf symbols */
--symbol_map __TI_STACK_SIZE=__STACK_SIZE
--symbol_map __TI_STACK_BASE=__stack
--symbol_map _stack=__stack


--args 0x0
-heap  0x0
-stack 0x300

/*
 * Linker command file contributions from all loaded packages:
 */

/* Content from xdc.services.global (null): */

/* Content from xdc (null): */

/* Content from xdc.corevers (null): */

/* Content from xdc.shelf (null): */

/* Content from xdc.services.spec (null): */

/* Content from xdc.services.intern.xsr (null): */

/* Content from xdc.services.intern.gen (null): */

/* Content from xdc.services.intern.cmd (null): */

/* Content from xdc.bld (null): */

/* Content from ti.targets (null): */

/* Content from ti.targets.arm.elf (null): */

/* Content from xdc.rov (null): */

/* Content from xdc.runtime (null): */

/* Content from ti.targets.arm.rtsarm (null): */

/* Content from ti.sysbios.interfaces (null): */

/* Content from ti.sysbios.family (null): */

/* Content from ti.sysbios.family.arm (ti/sysbios/family/arm/linkcmd.xdt): */
--retain "*(.vecs)"

/* Content from xdc.services.getset (null): */

/* Content from ti.sysbios.rom (null): */

/* Content from ti.sysbios.rts (ti/sysbios/rts/linkcmd.xdt): */

/* Content from xdc.runtime.knl (null): */

/* Content from ti.catalog.arm.cortexm3 (null): */

/* Content from ti.catalog.peripherals.hdvicp2 (null): */

/* Content from ti.catalog (null): */

/* Content from ti.catalog.arm.peripherals.timers (null): */

/* Content from xdc.platform (null): */

/* Content from xdc.cfg (null): */

/* Content from ti.catalog.arm.cortexm4 (null): */

/* Content from ti.platforms.simplelink (null): */

/* Content from ti.sysbios.hal (null): */

/* Content from ti.sysbios.family.arm.cc26xx (null): */

/* Content from ti.sysbios.family.arm.m3 (ti/sysbios/family/arm/m3/linkcmd.xdt): */
-u _c_int00
--retain "*(.resetVecs)"
ti_sysbios_family_arm_m3_Hwi_nvic = 0xe000e000;

/* Content from ti.sysbios.knl (null): */

/* Content from ti.sysbios (null): */

/* Content from ti.drivers.ports (null): */

/* Content from ti.mw.fatfs (null): */

/* Content from ti.sysbios.gates (null): */

/* Content from ti.sysbios.xdcruntime (null): */

/* Content from ti.sysbios.heaps (null): */

/* Content from ti.sysbios.rom.cortexm.cc13xx (/Users/mikael/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/bios_6_46_01_37/packages/ti/sysbios/rom/cortexm/cc13xx/golden/CC13xx/CC13xx_link_ti.xdt): */

-u xdc_runtime_Error_policy__C
-u xdc_runtime_IModule_Interface__BASE__C
-u xdc_runtime_Startup_lastFxns__C
-u ti_sysbios_gates_GateMutex_Object__DESC__C
-u ti_sysbios_rom_ROM_ti_sysbios_family_arm_cc26xx_Timer_initDevice__I
-u xdc_runtime_Startup_execImpl__C
-u ti_sysbios_gates_GateMutex_Instance_State_sem__O
-u ti_sysbios_rom_ROM_ti_sysbios_family_arm_cc26xx_Timer_getMaxTicks__E
-u ti_sysbios_knl_Swi_Object__count__C
-u ti_sysbios_knl_Idle_funcList__C
-u ti_sysbios_family_arm_m3_Hwi_Object__PARAMS__C
-u xdc_runtime_Text_isLoaded__C
-u ti_sysbios_knl_Clock_Object__DESC__C
-u ti_sysbios_knl_Mailbox_Instance_State_dataQue__O
-u ti_sysbios_gates_GateMutex_Module__FXNS__C
-u ti_sysbios_knl_Task_Module_State_inactiveQ__O
-u ti_sysbios_family_arm_m3_Hwi_Module__id__C
-u ti_sysbios_family_arm_cc26xx_Timer_Module__id__C
-u ti_sysbios_knl_Mailbox_Object__table__C
-u ti_sysbios_family_arm_m3_Hwi_Object__table__C
-u ti_sysbios_knl_Swi_Object__DESC__C
-u xdc_runtime_Text_charCnt__C
-u ti_sysbios_rom_ROM_ti_sysbios_family_arm_cc26xx_Timer_start__E
-u ti_sysbios_heaps_HeapMem_Object__table__C
-u xdc_runtime_Error_policyFxn__C
-u ti_sysbios_rom_ROM_ti_sysbios_family_arm_cc26xx_Timer_getCount64__E
-u xdc_runtime_Startup_firstFxns__C
-u ti_sysbios_knl_Swi_Object__PARAMS__C
-u ti_sysbios_knl_Clock_serviceMargin__C
-u xdc_runtime_Text_charTab__C
-u ti_sysbios_rom_ROM_AONRTCCurrentCompareValueGet
-u ti_sysbios_rom_ROM_ti_sysbios_family_arm_cc26xx_TimestampProvider_get32__E
-u ti_sysbios_rom_ROM_ti_sysbios_family_arm_cc26xx_Timer_getCurrentTick__E
-u ti_sysbios_family_arm_m3_TaskSupport_stackAlignment__C
-u ti_sysbios_family_arm_m3_Hwi_NUM_INTERRUPTS__C
-u xdc_runtime_Main_Module__diagsMask__C
-u ti_sysbios_knl_Swi_Object__table__C
-u xdc_runtime_Memory_Module__id__C
-u ti_sysbios_knl_Task_Object__PARAMS__C
-u ti_sysbios_gates_GateMutex_Object__PARAMS__C
-u ti_sysbios_heaps_HeapMem_Module__gateObj__C
-u ti_sysbios_family_arm_cc26xx_Timer_startupNeeded__C
-u ti_sysbios_knl_Queue_Object__DESC__C
-u ti_sysbios_knl_Task_Object__DESC__C
-u xdc_runtime_Assert_E_assertFailed__C
-u ti_sysbios_heaps_HeapMem_Object__PARAMS__C
-u ti_sysbios_gates_GateHwi_Module__id__C
-u ti_sysbios_gates_GateHwi_Object__PARAMS__C
-u xdc_runtime_IHeap_Interface__BASE__C
-u xdc_runtime_SysCallback_exitFxn__C
-u ti_sysbios_heaps_HeapMem_Module__id__C
-u ti_sysbios_family_arm_m3_Hwi_excHandlerFunc__C
-u ti_sysbios_heaps_HeapMem_Module__FXNS__C
-u xdc_runtime_System_maxAtexitHandlers__C
-u ti_sysbios_knl_Queue_Object__count__C
-u ti_sysbios_knl_Task_Object__table__C
-u ti_sysbios_knl_Mailbox_Object__DESC__C
-u ti_sysbios_family_arm_m3_Hwi_nullIsrFunc__C
-u ti_sysbios_knl_Clock_tickMode__C
-u ti_sysbios_gates_GateMutex_Module__id__C
-u ti_sysbios_knl_Swi_numPriorities__C
-u ti_sysbios_knl_Task_numConstructedTasks__C
-u xdc_runtime_Startup_maxPasses__C
-u ti_sysbios_rom_ROM_AONRTCEventClear
-u ti_sysbios_knl_Task_initStackFlag__C
-u xdc_runtime_Main_Module__diagsEnabled__C
-u xdc_runtime_Main_Module__diagsIncluded__C
-u xdc_runtime_System_abortFxn__C
-u ti_sysbios_knl_Mailbox_Instance_State_dataSem__O
-u ti_sysbios_gates_GateHwi_Module__FXNS__C
-u ti_sysbios_hal_Hwi_Object__DESC__C
-u ti_sysbios_family_arm_m3_Hwi_priGroup__C
-u xdc_runtime_Error_E_memory__C
-u ti_sysbios_family_arm_m3_Hwi_E_alreadyDefined__C
-u ti_sysbios_knl_Mailbox_Instance_State_freeSem__O
-u ti_sysbios_knl_Queue_Object__table__C
-u ti_sysbios_knl_Semaphore_Object__PARAMS__C
-u xdc_runtime_System_exitFxn__C
-u ti_sysbios_knl_Clock_Object__PARAMS__C
-u ti_sysbios_rom_ROM_AONRTCCompareValueSet
-u ti_sysbios_rom_ROM_ti_sysbios_family_arm_cc26xx_Timer_setNextTick__E
-u ti_sysbios_heaps_HeapMem_reqAlign__C
-u xdc_runtime_Main_Module__id__C
-u xdc_runtime_Startup_sfxnRts__C
-u ti_sysbios_knl_Semaphore_Object__DESC__C
-u ti_sysbios_gates_GateHwi_Object__DESC__C
-u ti_sysbios_heaps_HeapMem_Object__count__C
-u ti_sysbios_family_arm_m3_Hwi_numSparseInterrupts__C
-u ti_sysbios_knl_Mailbox_maxTypeAlign__C
-u ti_sysbios_family_arm_cc26xx_TimestampProvider_useClockTimer__C
-u ti_sysbios_rom_ROM_xdc_runtime_System_SupportProxy_exit__E
-u ti_sysbios_knl_Queue_Object__PARAMS__C
-u ti_sysbios_knl_Task_allBlockedFunc__C
-u ti_sysbios_rom_ROM_xdc_runtime_System_SupportProxy_abort__E
-u ti_sysbios_knl_Mailbox_Object__count__C
-u xdc_runtime_Text_nameStatic__C
-u ti_sysbios_rom_ROM_xdc_runtime_Startup_getState__I
-u ti_sysbios_knl_Clock_Module_State_clockQ__O
-u ti_sysbios_knl_Task_defaultStackSize__C
-u xdc_runtime_IGateProvider_Interface__BASE__C
-u ti_sysbios_family_arm_m3_Hwi_E_hwiLimitExceeded__C
-u xdc_runtime_Startup_startModsFxn__C
-u ti_sysbios_knl_Semaphore_Instance_State_pendQ__O
-u ti_sysbios_family_arm_m3_Hwi_Object__DESC__C
-u xdc_runtime_Text_nameEmpty__C
-u ti_sysbios_family_arm_m3_Hwi_Object__count__C
-u xdc_runtime_SysCallback_abortFxn__C
-u ti_sysbios_knl_Task_defaultStackHeap__C
-u ti_sysbios_family_arm_m3_Hwi_ccr__C
-u ti_sysbios_knl_Mailbox_Object__PARAMS__C
-u ti_sysbios_hal_Hwi_Object__PARAMS__C
-u ti_sysbios_heaps_HeapMem_E_memory__C
-u ti_sysbios_knl_Task_Object__count__C
-u ti_sysbios_rom_ROM_AONRTCChannelEnable
-u ti_sysbios_heaps_HeapMem_Object__DESC__C
-u xdc_runtime_Text_nameUnknown__C
-u xdc_runtime_Memory_defaultHeapInstance__C
-u ti_sysbios_knl_Mailbox_Instance_State_freeQue__O
-u ti_sysbios_rom_ROM_ti_sysbios_family_arm_cc26xx_Timer_setThreshold__I
-u xdc_runtime_Startup_sfxnTab__C
-u ti_sysbios_knl_Clock_Module__state__V
-u ti_sysbios_family_arm_cc26xx_TimestampProvider_Module__state__V
-u xdc_runtime_Startup_Module__state__V
-u ti_sysbios_BIOS_Module__state__V
-u ti_sysbios_knl_Swi_Module__state__V
-u ti_sysbios_knl_Task_Module__state__V
-u xdc_runtime_Memory_Module__state__V
-u xdc_runtime_System_Module__state__V
-u ti_sysbios_family_arm_m3_Hwi_Module__state__V
-u ti_sysbios_family_arm_cc26xx_Timer_Module__state__V

ti_sysbios_rom_cortexm_cc13xx_CC13xx_getRevision__E = 0x1001ca97;
ti_sysbios_knl_Queue_get__E = 0x1001bf0d;
ti_sysbios_knl_Swi_enabled__E = 0x1001c0e1;
ti_sysbios_knl_Clock_scheduleNextTick__E = 0x1001beb9;
ti_sysbios_knl_Swi_runLoop__I = 0x1001b0d5;
ti_sysbios_knl_Clock_getTicks__E = 0x1001b6d5;
ti_sysbios_gates_GateMutex_Object__destruct__S = 0x1001bc45;
ti_sysbios_knl_Queue_enqueue__E = 0x1001c1b1;
ti_sysbios_knl_Queue_put__E = 0x1001bf29;
ti_sysbios_family_arm_m3_Hwi_Object__create__S = 0x1001aab1;
ti_sysbios_gates_GateHwi_Instance_init__E = 0x1001b45f;
ti_sysbios_hal_Hwi_Instance_finalize__E = 0x1001c1ed;
ti_sysbios_BIOS_RtsGateProxy_leave__E = 0x1001c249;
ti_sysbios_heaps_HeapMem_Object__create__S = 0x1001b841;
xdc_runtime_Error_raiseX__E = 0x1001c765;
ti_sysbios_knl_Semaphore_construct = 0x1001b705;
ti_sysbios_knl_Clock_Object__destruct__S = 0x1001bce5;
ti_sysbios_knl_Clock_TimerProxy_getMaxTicks__E = 0x1001c0a1;
ti_sysbios_knl_Swi_Object__destruct__S = 0x1001bf45;
ti_sysbios_family_arm_cc26xx_TimestampProvider_getFreq__E = 0x1001c12b;
ti_sysbios_gates_GateMutex_Handle__label__S = 0x1001bc25;
ti_sysbios_knl_Mailbox_delete = 0x1001c2c1;
ti_sysbios_knl_Semaphore_destruct = 0x1001c2dd;
ti_sysbios_BIOS_RtsGateProxy_enter__E = 0x1001c245;
ti_sysbios_knl_Task_processVitalTaskFlag__I = 0x1001b735;
ti_sysbios_knl_Mailbox_create = 0x1001abdd;
xdc_runtime_Core_deleteObject__I = 0x1001c4f5;
ti_sysbios_knl_Queue_delete = 0x1001c2c9;
ti_sysbios_family_arm_m3_Hwi_doSwiRestore__I = 0x1001c1f7;
xdc_runtime_System_atexit__E = 0x1001c67d;
ti_sysbios_gates_GateMutex_Params__init__S = 0x1001c175;
ti_sysbios_knl_Clock_getTimerHandle__E = 0x1001c18d;
ti_sysbios_knl_Task_enable__E = 0x1001c23f;
ti_sysbios_knl_Clock_TimerProxy_getExpiredTicks__E = 0x1001c2a9;
ti_sysbios_knl_Queue_Object__destruct__S = 0x1001bef1;
ti_sysbios_knl_Clock_Object__delete__S = 0x1001bad9;
ti_sysbios_gates_GateMutex_delete = 0x1001c269;
ti_sysbios_heaps_HeapMem_restore__E = 0x1001c101;
ti_sysbios_knl_Swi_create = 0x1001ad05;
ti_sysbios_heaps_HeapMem_Module_GateProxy_leave__E = 0x1001c29d;
ti_sysbios_knl_Semaphore_pend__E = 0x1001a001;
ti_sysbios_knl_Mailbox_Instance_finalize__E = 0x1001a8f9;
xdc_runtime_Startup_startMods__I = 0x1001c309;
ti_sysbios_heaps_HeapMem_init__I = 0x1001b541;
ti_sysbios_knl_Swi_Object__delete__S = 0x1001bda5;
ti_sysbios_hal_Hwi_HwiProxy_enableInterrupt__E = 0x1001c281;
ti_sysbios_knl_Clock_removeI__E = 0x1001c2b9;
xdc_runtime_System_abort__E = 0x1001c745;
ti_sysbios_family_arm_m3_Hwi_dispatchC__I = 0x1001aa49;
ti_sysbios_knl_Swi_construct = 0x1001b085;
ti_sysbios_knl_Task_sleepTimeout__I = 0x1001bf7d;
ti_sysbios_knl_Queue_remove__E = 0x1001c11d;
ti_sysbios_knl_Semaphore_Instance_finalize__E = 0x1001c0c1;
ti_sysbios_gates_GateMutex_destruct = 0x1001c26d;
ti_sysbios_knl_Task_SupportProxy_Module__startupDone__S = 0x1001c2ed;
ti_sysbios_knl_Queue_Object__delete__S = 0x1001bd45;
ti_sysbios_knl_Mailbox_Object__get__S = 0x1001b899;
ti_sysbios_family_arm_m3_Hwi_Instance_init__E = 0x1001a3c5;
ti_sysbios_knl_Clock_delete = 0x1001c2b1;
ti_sysbios_knl_Clock_walkQueueDynamic__E = 0x1001a809;
ti_sysbios_knl_Mailbox_Object__destruct__S = 0x1001bed5;
ti_sysbios_knl_Mailbox_post__E = 0x1001a58d;
ti_sysbios_knl_Clock_Instance_init__E = 0x1001b509;
ti_sysbios_knl_Task_allBlockedFunction__I = 0x1001b361;
ti_sysbios_knl_Task_postInit__I = 0x1001a611;
ti_sysbios_knl_Task_enter__I = 0x1001be05;
ti_sysbios_hal_Hwi_switchFromBootStack__E = 0x1001c295;
ti_sysbios_knl_Semaphore_Object__destruct__S = 0x1001bd85;
ti_sysbios_hal_Hwi_HwiProxy_Object__create__S = 0x1001c081;
ti_sysbios_family_arm_cc26xx_Timer_postInit__I = 0x1001b765;
ti_sysbios_knl_Swi_Module_startup__E = 0x1001c233;
ti_sysbios_gates_GateMutex_Instance_finalize__E = 0x1001c071;
xdc_runtime_Core_assignParams__I = 0x1001c5ed;
ti_sysbios_hal_Hwi_HwiProxy_switchFromBootStack__E = 0x1001c28d;
ti_sysbios_knl_Swi_post__E = 0x1001b31d;
ti_sysbios_hal_Hwi_initStack = 0x1001b815;
xdc_runtime_Memory_alloc__E = 0x1001c485;
ti_sysbios_knl_Queue_next__E = 0x1001c2d5;
ti_sysbios_knl_Clock_Instance_finalize__E = 0x1001bfc9;
ti_sysbios_knl_Queue_elemClear__E = 0x1001c22d;
ti_sysbios_knl_Clock_Params__init__S = 0x1001c181;
ti_sysbios_knl_Task_Instance_init__E = 0x1001a259;
ti_sysbios_hal_Hwi_HwiProxy_startup__E = 0x1001c289;
ti_sysbios_knl_Task_self__E = 0x1001c1e1;
ti_sysbios_knl_Task_startup__E = 0x1001c305;
ti_sysbios_gates_GateHwi_Object__delete__S = 0x1001bc05;
ti_sysbios_family_arm_cc26xx_Timer_startup__E = 0x1001b791;
xdc_runtime_Memory_free__E = 0x1001c7fd;
ti_sysbios_hal_Hwi_delete = 0x1001c291;
ti_sysbios_knl_Queue_Instance_init__E = 0x1001c227;
ti_sysbios_family_arm_cc26xx_Timer_Module_startup__E = 0x1001ba6d;
xdc_runtime_Assert_raise__I = 0x1001c5a5;
ti_sysbios_hal_Hwi_create = 0x1001ab79;
ti_sysbios_knl_Task_destruct = 0x1001c301;
ti_sysbios_hal_Hwi_Module_startup__E = 0x1001c01d;
ti_sysbios_family_arm_m3_Hwi_excHandler__I = 0x1001bbc5;
xdc_runtime_Core_destructObject__I = 0x1001c79d;
ti_sysbios_knl_Swi_disable__E = 0x1001c0d1;
ti_sysbios_BIOS_setThreadType__E = 0x1001c041;
ti_sysbios_knl_Task_disable__E = 0x1001c0f1;
ti_sysbios_knl_Swi_Instance_init__E = 0x1001aca5;
ti_sysbios_knl_Semaphore_pendTimeout__I = 0x1001b949;
ti_sysbios_knl_Clock_create = 0x1001b3e9;
ti_sysbios_knl_Idle_loop__E = 0x1001c207;
ti_sysbios_gates_GateHwi_leave__E = 0x1001c21b;
ti_sysbios_family_arm_m3_Hwi_enableInterrupt__E = 0x1001b295;
ti_sysbios_knl_Semaphore_Params__init__S = 0x1001c1bd;
ti_sysbios_knl_Task_unblock__E = 0x1001bfb1;
ti_sysbios_knl_Swi_destruct = 0x1001c2e5;
ti_sysbios_BIOS_getCpuFreq__E = 0x1001bff5;
xdc_runtime_Memory_calloc__E = 0x1001c849;
ti_sysbios_family_arm_m3_Hwi_startup__E = 0x1001c1ff;
xdc_runtime_SysCallback_exit__E = 0x1001c81d;
ti_sysbios_knl_Queue_empty__E = 0x1001c10f;
ti_sysbios_knl_Clock_logTick__E = 0x1001be63;
ti_sysbios_knl_Task_yield__E = 0x1001b3a5;
ti_sysbios_knl_Task_SupportProxy_getStackAlignment__E = 0x1001c2f1;
ti_sysbios_family_arm_m3_Hwi_create = 0x1001ab15;
xdc_runtime_Timestamp_SupportProxy_get32__E = 0x1001c82d;
ti_sysbios_family_arm_m3_Hwi_destruct = 0x1001c25d;
ti_sysbios_family_arm_m3_Hwi_doTaskRestore__I = 0x1001c20f;
ti_sysbios_knl_Swi_run__I = 0x1001afd9;
ti_sysbios_family_arm_cc26xx_Timer_Module__startupDone__S = 0x1001bb45;
xdc_runtime_Core_createObject__I = 0x1001c401;
ti_sysbios_knl_Queue_create = 0x1001b91d;
ti_sysbios_hal_Hwi_Object__delete__S = 0x1001bc65;
ti_sysbios_knl_Clock_construct = 0x1001b575;
xdc_runtime_System_abortSpin__E = 0x1001c895;
ti_sysbios_family_arm_m3_Hwi_Object__destruct__S = 0x1001be65;
ti_sysbios_hal_Hwi_HwiProxy_delete = 0x1001c279;
ti_sysbios_gates_GateMutex_Object__create__S = 0x1001b645;
ti_sysbios_family_arm_m3_Hwi_getStackInfo__E = 0x1001b461;
ti_sysbios_heaps_HeapMem_Module_GateProxy_enter__E = 0x1001c299;
ti_sysbios_knl_Semaphore_post__E = 0x1001ac41;
ti_sysbios_knl_Task_exit__E = 0x1001b425;
ti_sysbios_heaps_HeapMem_Instance_init__E = 0x1001b4d1;
ti_sysbios_knl_Swi_restore__E = 0x1001b5dd;
ti_sysbios_knl_Task_startCore__E = 0x1001ae75;
ti_sysbios_knl_Semaphore_create = 0x1001b5a9;
ti_sysbios_gates_GateHwi_enter__E = 0x1001c169;
ti_sysbios_knl_Task_blockI__E = 0x1001b611;
ti_sysbios_heaps_HeapMem_free__E = 0x1001a791;
ti_sysbios_knl_Task_Object__destruct__S = 0x1001bf61;
ti_sysbios_family_arm_cc26xx_Timer_periodicStub__E = 0x1001a465;
ti_sysbios_hal_Hwi_Instance_init__E = 0x1001b9f5;
ti_sysbios_gates_GateHwi_query__E = 0x1001c265;
xdc_runtime_System_processAtExit__E = 0x1001c6b5;
ti_sysbios_family_arm_cc26xx_Timer_setPeriod__E = 0x1001c255;
xdc_runtime_Error_init__E = 0x1001c83d;
ti_sysbios_knl_Semaphore_Instance_init__E = 0x1001bafd;
ti_sysbios_knl_Queue_head__E = 0x1001c2d1;
xdc_runtime_Error_check__E = 0x1001c7b5;
xdc_runtime_Error_policySpin__E = 0x1001c483;
ti_sysbios_gates_GateMutex_create = 0x1001b675;
xdc_runtime_Gate_leaveSystem__E = 0x1001c875;
ti_sysbios_knl_Swi_restoreHwi__E = 0x1001ad61;
ti_sysbios_knl_Task_sleep__E = 0x1001a9d9;
ti_sysbios_knl_Task_create = 0x1001ae19;
ti_sysbios_knl_Mailbox_Params__init__S = 0x1001c199;
ti_sysbios_knl_Task_restoreHwi__E = 0x1001be25;
ti_sysbios_knl_Mailbox_postInit__I = 0x1001b8c5;
ti_sysbios_knl_Task_delete = 0x1001c2fd;
ti_sysbios_heaps_HeapMem_isBlocking__E = 0x1001c2a1;
ti_sysbios_knl_Clock_startI__E = 0x1001a715;
ti_sysbios_knl_Clock_start__E = 0x1001bfdf;
ti_sysbios_family_arm_m3_Hwi_Object__delete__S = 0x1001bba5;
ti_sysbios_knl_Clock_TimerProxy_getPeriod__E = 0x1001c2ad;
ti_sysbios_knl_Task_SupportProxy_start__E = 0x1001c2f5;
ti_sysbios_heaps_HeapMem_Handle__label__S = 0x1001bc85;
ti_sysbios_family_arm_m3_Hwi_delete = 0x1001c259;
ti_sysbios_knl_Semaphore_Object__delete__S = 0x1001bb21;
ti_sysbios_hal_Hwi_HwiProxy_getStackInfo__E = 0x1001c285;
ti_sysbios_knl_Idle_run__E = 0x1001bd05;
ti_sysbios_knl_Swi_delete = 0x1001c2e1;
xdc_runtime_Memory_valloc__E = 0x1001c781;
ti_sysbios_knl_Mailbox_Object__delete__S = 0x1001bd25;
ti_sysbios_family_arm_m3_TaskSupport_start__E = 0x1001b499;
ti_sysbios_family_arm_m3_Hwi_Module__startupDone__S = 0x1001bb85;
ti_sysbios_knl_Swi_startup__E = 0x1001c239;
ti_sysbios_knl_Task_schedule__I = 0x1001b175;
ti_sysbios_gates_GateMutex_leave__E = 0x1001bf99;
ti_sysbios_heaps_HeapMem_Object__delete__S = 0x1001bca5;
ti_sysbios_knl_Clock_TimerProxy_setNextTick__E = 0x1001c0b1;
ti_sysbios_knl_Swi_Object__get__S = 0x1001b975;
ti_sysbios_knl_Task_restore__E = 0x1001ba45;
xdc_runtime_Memory_HeapProxy_alloc__E = 0x1001c885;
ti_sysbios_gates_GateHwi_Object__create__S = 0x1001ba91;
ti_sysbios_hal_Hwi_HwiProxy_disableInterrupt__E = 0x1001c27d;
ti_sysbios_BIOS_start__E = 0x1001c051;
ti_sysbios_BIOS_exit__E = 0x1001c031;
ti_sysbios_family_arm_m3_TaskSupport_getStackAlignment__E = 0x1001c15d;
xdc_runtime_SysCallback_abort__E = 0x1001c80d;
ti_sysbios_knl_Queue_destruct = 0x1001c2cd;
ti_sysbios_family_arm_m3_Hwi_postInit__I = 0x1001a315;
ti_sysbios_gates_GateMutex_Instance_init__E = 0x1001be9d;
ti_sysbios_knl_Task_Instance_finalize__E = 0x1001a4f9;
ti_sysbios_knl_Clock_TimerProxy_getCurrentTick__E = 0x1001c091;
ti_sysbios_family_arm_m3_Hwi_disableFxn__E = 0x1001c145;
xdc_runtime_Memory_HeapProxy_free__E = 0x1001c889;
ti_sysbios_knl_Mailbox_Module_startup__E = 0x1001b2d9;
ti_sysbios_knl_Task_Object__delete__S = 0x1001bdc5;
ti_sysbios_gates_GateHwi_Handle__label__S = 0x1001bbe5;
xdc_runtime_Text_ropeText__E = 0x1001c7e5;
ti_sysbios_knl_Clock_destruct = 0x1001c2b5;
ti_sysbios_knl_Queue_construct = 0x1001b8f1;
ti_sysbios_family_arm_m3_Hwi_switchFromBootStack__E = 0x1001c009;
ti_sysbios_heaps_HeapMem_Object__get__S = 0x1001b86d;
ti_sysbios_hal_Hwi_HwiProxy_create = 0x1001be45;
ti_sysbios_gates_GateMutex_query__E = 0x1001c271;
ti_sysbios_knl_Swi_schedule__I = 0x1001b031;
ti_sysbios_knl_Task_Params__init__S = 0x1001c1d5;
ti_sysbios_family_arm_m3_Hwi_Params__init__S = 0x1001c139;
ti_sysbios_family_arm_m3_Hwi_plug__E = 0x1001c061;
xdc_runtime_System_exitSpin__E = 0x1001c897;
ti_sysbios_gates_GateMutex_construct = 0x1001b7e9;
xdc_runtime_System_Module_GateProxy_leave__E = 0x1001c891;
ti_sysbios_knl_Mailbox_pend__E = 0x1001a969;
ti_sysbios_family_arm_m3_TaskSupport_Module__startupDone__S = 0x1001c261;
xdc_runtime_Core_assignLabel__I = 0x1001c6e9;
xdc_runtime_System_Module_GateProxy_enter__E = 0x1001c88d;
xdc_runtime_System_exit__E = 0x1001c7cd;
ti_sysbios_knl_Swi_Params__init__S = 0x1001c1c9;
ti_sysbios_knl_Clock_workFunc__E = 0x1001a881;
ti_sysbios_family_arm_m3_Hwi_restoreFxn__E = 0x1001c215;
ti_sysbios_family_arm_cc26xx_TimestampProvider_Module_startup__E = 0x1001bb65;
ti_sysbios_knl_Semaphore_delete = 0x1001c2d9;
ti_sysbios_family_arm_cc26xx_Timer_getPeriod__E = 0x1001c251;
ti_sysbios_family_arm_m3_Hwi_initNVIC__E = 0x1001a695;
ti_sysbios_knl_Clock_addI__E = 0x1001ba1d;
ti_sysbios_family_arm_m3_Hwi_Instance_finalize__E = 0x1001aed1;
ti_sysbios_heaps_HeapMem_alloc__E = 0x1001a195;
ti_sysbios_knl_Task_unblockI__E = 0x1001b9a1;
ti_sysbios_knl_Swi_Instance_finalize__E = 0x1001bf97;
ti_sysbios_family_arm_m3_Hwi_disableInterrupt__E = 0x1001b251;
ti_sysbios_family_arm_m3_Hwi_enableFxn__E = 0x1001c151;
xdc_runtime_Gate_enterSystem__E = 0x1001c881;
ti_sysbios_gates_GateMutex_Object__delete__S = 0x1001bab5;
ti_sysbios_family_arm_cc26xx_TimestampProvider_get64__E = 0x1001b9cd;
ti_sysbios_knl_Mailbox_Instance_init__E = 0x1001a0cd;
xdc_runtime_Text_cordText__E = 0x1001c719;
xdc_runtime_Startup_exec__E = 0x1001c555;
ti_sysbios_hal_Hwi_HwiProxy_Module__startupDone__S = 0x1001c275;
ti_sysbios_heaps_HeapMem_getStats__E = 0x1001b209;
ti_sysbios_knl_Task_SupportProxy_swap__E = 0x1001c2f9;
xdc_runtime_Memory_getMaxDefaultTypeAlign__E = 0x1001c855;
ti_sysbios_knl_Task_Object__get__S = 0x1001bde5;
ti_sysbios_family_arm_m3_Hwi_construct = 0x1001af29;
ti_sysbios_knl_Clock_TimerProxy_Module__startupDone__S = 0x1001c2a5;
ti_sysbios_knl_Clock_Module_startup__E = 0x1001bcc5;
ti_sysbios_knl_Mailbox_construct = 0x1001af81;
ti_sysbios_knl_Task_construct = 0x1001b125;
xdc_runtime_Core_constructObject__I = 0x1001c635;
ti_sysbios_knl_Queue_dequeue__E = 0x1001c1a5;
ti_sysbios_knl_Task_Module_startup__E = 0x1001adbd;
ti_sysbios_family_arm_cc26xx_Timer_getExpiredTicks__E = 0x1001c24d;
ti_sysbios_family_arm_m3_Hwi_Object__get__S = 0x1001b7bd;
ti_sysbios_knl_Mailbox_destruct = 0x1001c2c5;
xdc_runtime_System_Module_startup__E = 0x1001c87b;
ti_sysbios_knl_Swi_postInit__I = 0x1001c2e9;
ti_sysbios_family_arm_m3_Hwi_Module_startup__E = 0x1001b1c1;
ti_sysbios_gates_GateMutex_enter__E = 0x1001b6a5;
ti_sysbios_family_arm_m3_Hwi_setPriority__E = 0x1001be81;
ti_sysbios_knl_Queue_Object__get__S = 0x1001bd65;
ti_sysbios_knl_Clock_setTimeout__E = 0x1001c2bd;
ti_sysbios_knl_Task_swapReturn = 0x1001ca6d;
ti_sysbios_family_arm_m3_TaskSupport_glue = 0x1001ca55;
ti_sysbios_family_arm_m3_TaskSupport_buildTaskStack = 0x1001c969;
ti_sysbios_family_arm_m3_TaskSupport_swap__E = 0x1001ca65;
ti_sysbios_family_arm_m3_Hwi_excHandlerAsm__I = 0x1001c9fd;
ti_sysbios_family_arm_m3_Hwi_return = 0x1001ca53;
ti_sysbios_family_arm_m3_Hwi_pendSV__I = 0x1001ca3b;
ti_sysbios_family_arm_m3_Hwi_dispatch__I = 0x1001c899;
ti_sysbios_family_xxx_Hwi_switchAndRunFunc = 0x1001ca1d;
ti_sysbios_family_arm_m3_Hwi_initStacks__E = 0x1001c9b5;
ti_sysbios_BIOS_RtsGateProxy_Object__delete__S = 0x1001bab5;
ti_sysbios_BIOS_RtsGateProxy_Params__init__S = 0x1001c175;
ti_sysbios_BIOS_RtsGateProxy_Handle__label__S = 0x1001bc25;
ti_sysbios_BIOS_RtsGateProxy_query__E = 0x1001c271;
ti_sysbios_knl_Clock_TimerProxy_startup__E = 0x1001b791;
ti_sysbios_hal_Hwi_disableInterrupt__E = 0x1001c27d;
ti_sysbios_hal_Hwi_enableInterrupt__E = 0x1001c281;
ti_sysbios_hal_Hwi_getStackInfo__E = 0x1001c285;
ti_sysbios_hal_Hwi_startup__E = 0x1001c289;
ti_sysbios_hal_Hwi_HwiProxy_Object__delete__S = 0x1001bba5;
ti_sysbios_hal_Hwi_HwiProxy_Params__init__S = 0x1001c139;
ti_sysbios_heaps_HeapMem_Module_GateProxy_query__E = 0x1001c271;
ti_sysbios_heaps_HeapMem_Module_GateProxy_Object__delete__S = 0x1001bab5;
ti_sysbios_heaps_HeapMem_Module_GateProxy_Params__init__S = 0x1001c175;
ti_sysbios_heaps_HeapMem_Module_GateProxy_Handle__label__S = 0x1001bc25;
xdc_runtime_Timestamp_SupportProxy_get64__E = 0x1001b9cd;
xdc_runtime_Timestamp_SupportProxy_getFreq__E = 0x1001c12b;
xdc_runtime_Timestamp_get32__E = 0x1001c82d;
xdc_runtime_Timestamp_get64__E = 0x1001b9cd;
xdc_runtime_Timestamp_getFreq__E = 0x1001c12b;
xdc_runtime_Memory_HeapProxy_Object__delete__S = 0x1001bca5;
xdc_runtime_Memory_HeapProxy_Handle__label__S = 0x1001bc85;
xdc_runtime_System_Module_GateProxy_Object__delete__S = 0x1001bc05;
xdc_runtime_System_Module_GateProxy_Handle__label__S = 0x1001bbe5;
xdc_runtime_System_Module_GateProxy_query__E = 0x1001c265;

SECTIONS
{
    .const:xdc_runtime_Error_policy__C: LOAD > 0x00001490
    .const:xdc_runtime_IModule_Interface__BASE__C: LOAD > 0x0000143c
    .const:xdc_runtime_Startup_lastFxns__C: LOAD > 0x00001420
    .const:ti_sysbios_gates_GateMutex_Object__DESC__C: LOAD > 0x00001210
    .const:ti_sysbios_rom_ROM_ti_sysbios_family_arm_cc26xx_Timer_initDevice__I: LOAD > 0x000014b0
    .const:xdc_runtime_Startup_execImpl__C: LOAD > 0x00001450
    .const:ti_sysbios_gates_GateMutex_Instance_State_sem__O: LOAD > 0x000014d0
    .const:ti_sysbios_rom_ROM_ti_sysbios_family_arm_cc26xx_Timer_getMaxTicks__E: LOAD > 0x000014ac
    .const:ti_sysbios_knl_Swi_Object__count__C: LOAD > 0x000013e0
    .const:ti_sysbios_knl_Idle_funcList__C: LOAD > 0x00001378
    .const:ti_sysbios_family_arm_m3_Hwi_Object__PARAMS__C: LOAD > 0x00001070
    .const:xdc_runtime_Text_isLoaded__C: LOAD > 0x0000148e
    .const:ti_sysbios_knl_Clock_Object__DESC__C: LOAD > 0x00001270
    .const:ti_sysbios_knl_Mailbox_Instance_State_dataQue__O: LOAD > 0x000014d8
    .const:ti_sysbios_gates_GateMutex_Module__FXNS__C: LOAD > 0x00001140
    .const:ti_sysbios_knl_Task_Module_State_inactiveQ__O: LOAD > 0x000014ec
    .const:ti_sysbios_family_arm_m3_Hwi_Module__id__C: LOAD > 0x0000140a
    .const:ti_sysbios_family_arm_cc26xx_Timer_Module__id__C: LOAD > 0x00001406
    .const:ti_sysbios_knl_Mailbox_Object__table__C: LOAD > 0x000013d0
    .const:ti_sysbios_family_arm_m3_Hwi_Object__table__C: LOAD > 0x00001398
    .const:ti_sysbios_knl_Swi_Object__DESC__C: LOAD > 0x000012f0
    .const:xdc_runtime_Text_charCnt__C: LOAD > 0x0000148c
    .const:ti_sysbios_rom_ROM_ti_sysbios_family_arm_cc26xx_Timer_start__E: LOAD > 0x000014bc
    .const:ti_sysbios_heaps_HeapMem_Object__table__C: LOAD > 0x000013c0
    .const:xdc_runtime_Error_policyFxn__C: LOAD > 0x00001430
    .const:ti_sysbios_rom_ROM_ti_sysbios_family_arm_cc26xx_Timer_getCount64__E: LOAD > 0x000014a4
    .const:xdc_runtime_Startup_firstFxns__C: LOAD > 0x00001418
    .const:ti_sysbios_knl_Swi_Object__PARAMS__C: LOAD > 0x000010f4
    .const:ti_sysbios_knl_Clock_serviceMargin__C: LOAD > 0x000013c8
    .const:xdc_runtime_Text_charTab__C: LOAD > 0x00001478
    .const:ti_sysbios_rom_ROM_AONRTCCurrentCompareValueGet: LOAD > 0x0000149c
    .const:ti_sysbios_rom_ROM_ti_sysbios_family_arm_cc26xx_TimestampProvider_get32__E: LOAD > 0x000014c0
    .const:ti_sysbios_rom_ROM_ti_sysbios_family_arm_cc26xx_Timer_getCurrentTick__E: LOAD > 0x000014a8
    .const:ti_sysbios_family_arm_m3_TaskSupport_stackAlignment__C: LOAD > 0x000013b0
    .const:ti_sysbios_family_arm_m3_Hwi_NUM_INTERRUPTS__C: LOAD > 0x00001390
    .const:xdc_runtime_Main_Module__diagsMask__C: LOAD > 0x00001448
    .const:ti_sysbios_knl_Swi_Object__table__C: LOAD > 0x000013e4
    .const:xdc_runtime_Memory_Module__id__C: LOAD > 0x0000148a
    .const:ti_sysbios_knl_Task_Object__PARAMS__C: LOAD > 0x00001000
    .const:ti_sysbios_gates_GateMutex_Object__PARAMS__C: LOAD > 0x00001348
    .const:ti_sysbios_heaps_HeapMem_Module__gateObj__C: LOAD > 0x000013b8
    .const:ti_sysbios_family_arm_cc26xx_Timer_startupNeeded__C: LOAD > 0x00001384
    .const:ti_sysbios_knl_Queue_Object__DESC__C: LOAD > 0x000012b0
    .const:ti_sysbios_knl_Task_Object__DESC__C: LOAD > 0x00001310
    .const:xdc_runtime_Assert_E_assertFailed__C: LOAD > 0x00001428
    .const:ti_sysbios_heaps_HeapMem_Object__PARAMS__C: LOAD > 0x00001164
    .const:ti_sysbios_gates_GateHwi_Module__id__C: LOAD > 0x0000140c
    .const:ti_sysbios_gates_GateHwi_Object__PARAMS__C: LOAD > 0x00001330
    .const:xdc_runtime_IHeap_Interface__BASE__C: LOAD > 0x00001438
    .const:xdc_runtime_SysCallback_exitFxn__C: LOAD > 0x00001468
    .const:ti_sysbios_heaps_HeapMem_Module__id__C: LOAD > 0x00001410
    .const:ti_sysbios_family_arm_m3_Hwi_excHandlerFunc__C: LOAD > 0x000013a0
    .const:ti_sysbios_heaps_HeapMem_Module__FXNS__C: LOAD > 0x000010cc
    .const:xdc_runtime_System_maxAtexitHandlers__C: LOAD > 0x00001474
    .const:ti_sysbios_knl_Queue_Object__count__C: LOAD > 0x000013d8
    .const:ti_sysbios_knl_Task_Object__table__C: LOAD > 0x000013f0
    .const:ti_sysbios_knl_Mailbox_Object__DESC__C: LOAD > 0x00001290
    .const:ti_sysbios_family_arm_m3_Hwi_nullIsrFunc__C: LOAD > 0x000013a4
    .const:ti_sysbios_knl_Clock_tickMode__C: LOAD > 0x00001414
    .const:ti_sysbios_gates_GateMutex_Module__id__C: LOAD > 0x0000140e
    .const:ti_sysbios_knl_Swi_numPriorities__C: LOAD > 0x000013e8
    .const:ti_sysbios_knl_Task_numConstructedTasks__C: LOAD > 0x00001400
    .const:xdc_runtime_Startup_maxPasses__C: LOAD > 0x00001454
    .const:ti_sysbios_rom_ROM_AONRTCEventClear: LOAD > 0x000014a0
    .const:ti_sysbios_knl_Task_initStackFlag__C: LOAD > 0x00001412
    .const:xdc_runtime_Main_Module__diagsEnabled__C: LOAD > 0x00001440
    .const:xdc_runtime_Main_Module__diagsIncluded__C: LOAD > 0x00001444
    .const:xdc_runtime_System_abortFxn__C: LOAD > 0x0000146c
    .const:ti_sysbios_knl_Mailbox_Instance_State_dataSem__O: LOAD > 0x000014dc
    .const:ti_sysbios_gates_GateHwi_Module__FXNS__C: LOAD > 0x0000111c
    .const:ti_sysbios_hal_Hwi_Object__DESC__C: LOAD > 0x00001230
    .const:ti_sysbios_family_arm_m3_Hwi_priGroup__C: LOAD > 0x000013ac
    .const:xdc_runtime_Error_E_memory__C: LOAD > 0x0000142c
    .const:ti_sysbios_family_arm_m3_Hwi_E_alreadyDefined__C: LOAD > 0x00001388
    .const:ti_sysbios_knl_Mailbox_Instance_State_freeSem__O: LOAD > 0x000014e4
    .const:ti_sysbios_knl_Queue_Object__table__C: LOAD > 0x000013dc
    .const:ti_sysbios_knl_Semaphore_Object__PARAMS__C: LOAD > 0x000011ac
    .const:xdc_runtime_System_exitFxn__C: LOAD > 0x00001470
    .const:ti_sysbios_knl_Clock_Object__PARAMS__C: LOAD > 0x00001188
    .const:ti_sysbios_rom_ROM_AONRTCCompareValueSet: LOAD > 0x00001498
    .const:ti_sysbios_rom_ROM_ti_sysbios_family_arm_cc26xx_Timer_setNextTick__E: LOAD > 0x000014b4
    .const:ti_sysbios_heaps_HeapMem_reqAlign__C: LOAD > 0x000013c4
    .const:xdc_runtime_Main_Module__id__C: LOAD > 0x00001488
    .const:xdc_runtime_Startup_sfxnRts__C: LOAD > 0x00001458
    .const:ti_sysbios_knl_Semaphore_Object__DESC__C: LOAD > 0x000012d0
    .const:ti_sysbios_gates_GateHwi_Object__DESC__C: LOAD > 0x000011f0
    .const:ti_sysbios_heaps_HeapMem_Object__count__C: LOAD > 0x000013bc
    .const:ti_sysbios_family_arm_m3_Hwi_numSparseInterrupts__C: LOAD > 0x000013a8
    .const:ti_sysbios_knl_Mailbox_maxTypeAlign__C: LOAD > 0x000013d4
    .const:ti_sysbios_family_arm_cc26xx_TimestampProvider_useClockTimer__C: LOAD > 0x00001408
    .const:ti_sysbios_rom_ROM_xdc_runtime_System_SupportProxy_exit__E: LOAD > 0x000014cc
    .const:ti_sysbios_knl_Queue_Object__PARAMS__C: LOAD > 0x00001360
    .const:ti_sysbios_knl_Task_allBlockedFunc__C: LOAD > 0x000013f4
    .const:ti_sysbios_rom_ROM_xdc_runtime_System_SupportProxy_abort__E: LOAD > 0x000014c8
    .const:ti_sysbios_knl_Mailbox_Object__count__C: LOAD > 0x000013cc
    .const:xdc_runtime_Text_nameStatic__C: LOAD > 0x00001480
    .const:ti_sysbios_rom_ROM_xdc_runtime_Startup_getState__I: LOAD > 0x000014c4
    .const:ti_sysbios_knl_Clock_Module_State_clockQ__O: LOAD > 0x000014d4
    .const:ti_sysbios_knl_Task_defaultStackSize__C: LOAD > 0x000013fc
    .const:xdc_runtime_IGateProvider_Interface__BASE__C: LOAD > 0x00001434
    .const:ti_sysbios_family_arm_m3_Hwi_E_hwiLimitExceeded__C: LOAD > 0x0000138c
    .const:xdc_runtime_Startup_startModsFxn__C: LOAD > 0x00001460
    .const:ti_sysbios_knl_Semaphore_Instance_State_pendQ__O: LOAD > 0x000014e8
    .const:ti_sysbios_family_arm_m3_Hwi_Object__DESC__C: LOAD > 0x000011d0
    .const:xdc_runtime_Text_nameEmpty__C: LOAD > 0x0000147c
    .const:ti_sysbios_family_arm_m3_Hwi_Object__count__C: LOAD > 0x00001394
    .const:xdc_runtime_SysCallback_abortFxn__C: LOAD > 0x00001464
    .const:ti_sysbios_knl_Task_defaultStackHeap__C: LOAD > 0x000013f8
    .const:ti_sysbios_family_arm_m3_Hwi_ccr__C: LOAD > 0x0000139c
    .const:ti_sysbios_knl_Mailbox_Object__PARAMS__C: LOAD > 0x0000103c
    .const:ti_sysbios_hal_Hwi_Object__PARAMS__C: LOAD > 0x000010a0
    .const:ti_sysbios_heaps_HeapMem_E_memory__C: LOAD > 0x000013b4
    .const:ti_sysbios_knl_Task_Object__count__C: LOAD > 0x000013ec
    .const:ti_sysbios_rom_ROM_AONRTCChannelEnable: LOAD > 0x00001494
    .const:ti_sysbios_heaps_HeapMem_Object__DESC__C: LOAD > 0x00001250
    .const:xdc_runtime_Text_nameUnknown__C: LOAD > 0x00001484
    .const:xdc_runtime_Memory_defaultHeapInstance__C: LOAD > 0x0000144c
    .const:ti_sysbios_knl_Mailbox_Instance_State_freeQue__O: LOAD > 0x000014e0
    .const:ti_sysbios_rom_ROM_ti_sysbios_family_arm_cc26xx_Timer_setThreshold__I: LOAD > 0x000014b8
    .const:xdc_runtime_Startup_sfxnTab__C: LOAD > 0x0000145c
    .data:ti_sysbios_knl_Clock_Module__state__V: LOAD > 0x20000178
    .data:ti_sysbios_family_arm_cc26xx_TimestampProvider_Module__state__V: LOAD > 0x200001ec
    .data:xdc_runtime_Startup_Module__state__V: LOAD > 0x200001f0
    .data:ti_sysbios_BIOS_Module__state__V: LOAD > 0x200001a4
    .data:ti_sysbios_knl_Swi_Module__state__V: LOAD > 0x200001c8
    .data:ti_sysbios_knl_Task_Module__state__V: LOAD > 0x20000100
    .data:xdc_runtime_Memory_Module__state__V: LOAD > 0x20000200
    .data:xdc_runtime_System_Module__state__V: LOAD > 0x200001f8
    .data:ti_sysbios_family_arm_m3_Hwi_Module__state__V: LOAD > 0x20000144
    .data:ti_sysbios_family_arm_cc26xx_Timer_Module__state__V: LOAD > 0x200001e4
}

/* Content from ti.drivers.pdm (null): */

/* Content from ti.sysbios.utils (null): */

/* Content from ti.drivers (null): */

/* Content from ti.drivers.rf (null): */

/* Content from ti.mw.grlib (null): */

/* Content from ti.mw.lcd (null): */

/* Content from ti.mw.display (null): */

/* Content from ti.mw (null): */

/* Content from configPkg (null): */

/* Content from xdc.services.io (null): */



/*
 * symbolic aliases for static instance objects
 */
xdc_runtime_Startup__EXECFXN__C = 1;
xdc_runtime_Startup__RESETFXN__C = 1;


SECTIONS
{
    .bootVecs:  type = DSECT
    .resetVecs: load > 0x0
    .vecs: load > 0x20000000, type = NOLOAD



    xdc.meta: type = COPY
    xdc.noload: type = COPY
}
