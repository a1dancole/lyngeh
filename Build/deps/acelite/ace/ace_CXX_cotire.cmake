# cotire.cmake 1.8.0 generated file
# F:/DungeonCraft/Build/deps/acelite/ace/ace_CXX_cotire.cmake
set (COTIRE_ADDITIONAL_PREFIX_HEADER_IGNORE_EXTENSIONS "inc;inl;ipp")
set (COTIRE_ADDITIONAL_PREFIX_HEADER_IGNORE_PATH "")
set (COTIRE_CLEAN_ALL_TARGET_NAME "clean_cotire")
set (COTIRE_CLEAN_TARGET_SUFFIX "_clean_cotire")
set (COTIRE_CMAKE_MODULE_FILE "F:/DungeonCraft/Server/deps/cotire/CMake/cotire.cmake")
set (COTIRE_CMAKE_MODULE_VERSION "1.8.0")
set (COTIRE_DEBUG "OFF")
set (COTIRE_INTDIR "cotire")
set (COTIRE_MAXIMUM_NUMBER_OF_UNITY_INCLUDES "-j")
set (COTIRE_MINIMUM_NUMBER_OF_TARGET_SOURCES "2")
set (COTIRE_PCH_ALL_TARGET_NAME "all_pch")
set (COTIRE_PCH_MEMORY_SCALING_FACTOR "500")
set (COTIRE_PCH_TARGET_SUFFIX "_pch")
set (COTIRE_PREFIX_HEADER_FILENAME_SUFFIX "_prefix")
set (COTIRE_TARGETS_FOLDER "cotire")
set (COTIRE_TARGET_COMPILE_DEFINITIONS_DEBUG "CMAKE_INTDIR=\"Debug\";ace_EXPORTS;_CONF_DIR=$<1:\"C:/Program Files (x86)/AzerothCore\">;SCRIPTS;ENABLE_EXTRAS;ENABLE_VMAP_CHECKS;ACE_BUILD_DLL;_WIN64;_BUILD_DIRECTIVE=\"$(ConfigurationName)\";_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_WARNINGS;__STDC_LIMIT_MACROS;WIN32;_WINDOWS")
set (COTIRE_TARGET_COMPILE_DEFINITIONS_MINSIZEREL "CMAKE_INTDIR=\"MinSizeRel\";ace_EXPORTS;_CONF_DIR=$<1:\"C:/Program Files (x86)/AzerothCore\">;SCRIPTS;ENABLE_EXTRAS;ENABLE_VMAP_CHECKS;ACE_BUILD_DLL;_WIN64;_BUILD_DIRECTIVE=\"$(ConfigurationName)\";_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_WARNINGS;__STDC_LIMIT_MACROS;WIN32;_WINDOWS;NDEBUG")
set (COTIRE_TARGET_COMPILE_DEFINITIONS_RELEASE "CMAKE_INTDIR=\"Release\";ace_EXPORTS;_CONF_DIR=$<1:\"C:/Program Files (x86)/AzerothCore\">;SCRIPTS;ENABLE_EXTRAS;ENABLE_VMAP_CHECKS;ACE_BUILD_DLL;_WIN64;_BUILD_DIRECTIVE=\"$(ConfigurationName)\";_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_WARNINGS;__STDC_LIMIT_MACROS;WIN32;_WINDOWS;NDEBUG")
set (COTIRE_TARGET_COMPILE_DEFINITIONS_RELWITHDEBINFO "CMAKE_INTDIR=\"RelWithDebInfo\";ace_EXPORTS;_CONF_DIR=$<1:\"C:/Program Files (x86)/AzerothCore\">;SCRIPTS;ENABLE_EXTRAS;ENABLE_VMAP_CHECKS;ACE_BUILD_DLL;_WIN64;_BUILD_DIRECTIVE=\"$(ConfigurationName)\";_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_WARNINGS;__STDC_LIMIT_MACROS;WIN32;_WINDOWS;NDEBUG")
set (COTIRE_TARGET_COMPILE_FLAGS_DEBUG "/GR;/EHsc;/Zm500;/MDd;/Zi;/Ob0;/Od;/RTC1;/bigobj;/W0;/MP;-std:c++17")
set (COTIRE_TARGET_COMPILE_FLAGS_MINSIZEREL "/GR;/EHsc;/Zm500;/MD;/O1;/Ob1;/W0;/MP;-std:c++17")
set (COTIRE_TARGET_COMPILE_FLAGS_RELEASE "/GR;/EHsc;/Zm500;/MD;/O2;/Ob2;/W0;/MP;-std:c++17")
set (COTIRE_TARGET_COMPILE_FLAGS_RELWITHDEBINFO "/GR;/EHsc;/Zm500;/MD;/Zi;/O2;/Ob1;/W0;/MP;-std:c++17")
set (COTIRE_TARGET_CONFIGURATION_TYPES "Debug;Release;MinSizeRel;RelWithDebInfo")
set (COTIRE_TARGET_CXX_COMPILER_LAUNCHER "COTIRE_TARGET_CXX_COMPILER_LAUNCHER-NOTFOUND")
set (COTIRE_TARGET_IGNORE_PATH "F:/DungeonCraft/Server;F:/DungeonCraft/Build")
set (COTIRE_TARGET_INCLUDE_DIRECTORIES_DEBUG "F:/DungeonCraft/Server/deps/acelite;F:/DungeonCraft/Server/deps/zlib")
set (COTIRE_TARGET_INCLUDE_DIRECTORIES_MINSIZEREL "F:/DungeonCraft/Server/deps/acelite;F:/DungeonCraft/Server/deps/zlib")
set (COTIRE_TARGET_INCLUDE_DIRECTORIES_RELEASE "F:/DungeonCraft/Server/deps/acelite;F:/DungeonCraft/Server/deps/zlib")
set (COTIRE_TARGET_INCLUDE_DIRECTORIES_RELWITHDEBINFO "F:/DungeonCraft/Server/deps/acelite;F:/DungeonCraft/Server/deps/zlib")
set (COTIRE_TARGET_INCLUDE_PRIORITY_PATH "")
set (COTIRE_TARGET_LANGUAGE "CXX")
set (COTIRE_TARGET_MAXIMUM_NUMBER_OF_INCLUDES "-j")
set (COTIRE_TARGET_POST_UNDEFS "")
set (COTIRE_TARGET_PRE_UNDEFS "")
set (COTIRE_TARGET_SOURCES "ACE.cpp;ACE_crc32.cpp;ACE_crc_ccitt.cpp;ace_wchar.cpp;Activation_Queue.cpp;Active_Map_Manager.cpp;Addr.cpp;Argv_Type_Converter.cpp;Assert.cpp;Asynch_IO.cpp;Asynch_IO_Impl.cpp;Asynch_Pseudo_Task.cpp;ATM_Acceptor.cpp;ATM_Addr.cpp;ATM_Connector.cpp;ATM_Params.cpp;ATM_QoS.cpp;ATM_Stream.cpp;Atomic_Op.cpp;Auto_Event.cpp;Barrier.cpp;Base_Thread_Adapter.cpp;Based_Pointer_Repository.cpp;Basic_Stats.cpp;Basic_Types.cpp;Capabilities.cpp;CDR_Base.cpp;CDR_Size.cpp;CDR_Stream.cpp;Cleanup.cpp;Codecs.cpp;Codeset_IBM1047.cpp;Codeset_Registry.cpp;Codeset_Registry_db.cpp;Condition_Attributes.cpp;Condition_Recursive_Thread_Mutex.cpp;Condition_Thread_Mutex.cpp;Configuration.cpp;Configuration_Import_Export.cpp;Connection_Recycling_Strategy.cpp;Containers.cpp;Copy_Disabled.cpp;Date_Time.cpp;DEV.cpp;DEV_Addr.cpp;DEV_Connector.cpp;DEV_IO.cpp;Dev_Poll_Reactor.cpp;Dirent.cpp;Dirent_Selector.cpp;DLL.cpp;DLL_Manager.cpp;Dump.cpp;Dynamic.cpp;Dynamic_Message_Strategy.cpp;Dynamic_Service_Base.cpp;Dynamic_Service_Dependency.cpp;Encoding_Converter.cpp;Encoding_Converter_Factory.cpp;Event.cpp;Event_Handler.cpp;Event_Handler_Handle_Timeout_Upcall.cpp;Event_Base.cpp;FIFO.cpp;FIFO_Recv.cpp;FIFO_Recv_Msg.cpp;FIFO_Send.cpp;FIFO_Send_Msg.cpp;FILE.cpp;FILE_Addr.cpp;FILE_Connector.cpp;FILE_IO.cpp;File_Lock.cpp;Filecache.cpp;Flag_Manip.cpp;Framework_Component.cpp;Functor.cpp;Functor_String.cpp;Get_Opt.cpp;Handle_Ops.cpp;Handle_Set.cpp;Hashable.cpp;High_Res_Timer.cpp;ICMP_Socket.cpp;INET_Addr.cpp;Init_ACE.cpp;IO_Cntl_Msg.cpp;IO_SAP.cpp;IOStream.cpp;IPC_SAP.cpp;Lib_Find.cpp;Local_Memory_Pool.cpp;Local_Name_Space.cpp;Local_Tokens.cpp;Lock.cpp;Log_Category.cpp;Log_Msg.cpp;Log_Msg_Backend.cpp;Log_Msg_Callback.cpp;Log_Msg_IPC.cpp;Log_Msg_NT_Event_Log.cpp;Log_Msg_UNIX_Syslog.cpp;Log_Record.cpp;Logging_Strategy.cpp;LSOCK.cpp;LSOCK_Acceptor.cpp;LSOCK_CODgram.cpp;LSOCK_Connector.cpp;LSOCK_Dgram.cpp;LSOCK_Stream.cpp;Malloc.cpp;Malloc_Allocator.cpp;Manual_Event.cpp;MEM_Acceptor.cpp;MEM_Addr.cpp;MEM_Connector.cpp;MEM_IO.cpp;Mem_Map.cpp;MEM_SAP.cpp;MEM_Stream.cpp;Message_Block.cpp;Message_Queue.cpp;Message_Queue_NT.cpp;Message_Queue_Vx.cpp;Method_Request.cpp;MMAP_Memory_Pool.cpp;Monitor_Admin.cpp;Monitor_Admin_Manager.cpp;Monitor_Base.cpp;Monitor_Control_Action.cpp;Monitor_Control_Types.cpp;Monitor_Point_Registry.cpp;Monitor_Size.cpp;Msg_WFMO_Reactor.cpp;Multihomed_INET_Addr.cpp;Mutex.cpp;Name_Proxy.cpp;Name_Request_Reply.cpp;Name_Space.cpp;Naming_Context.cpp;Netlink_Addr.cpp;Notification_Queue.cpp;Notification_Strategy.cpp;NT_Service.cpp;Obchunk.cpp;Object_Manager.cpp;Object_Manager_Base.cpp;Obstack.cpp;OS_Errno.cpp;OS_Log_Msg_Attributes.cpp;OS_main.cpp;OS_NS_arpa_inet.cpp;OS_NS_ctype.cpp;OS_NS_dirent.cpp;OS_NS_dlfcn.cpp;OS_NS_errno.cpp;OS_NS_fcntl.cpp;OS_NS_math.cpp;OS_NS_netdb.cpp;OS_NS_poll.cpp;OS_NS_pwd.cpp;OS_NS_regex.cpp;OS_NS_signal.cpp;OS_NS_stdio.cpp;OS_NS_stdlib.cpp;OS_NS_string.cpp;OS_NS_strings.cpp;OS_NS_stropts.cpp;OS_NS_sys_mman.cpp;OS_NS_sys_msg.cpp;OS_NS_sys_resource.cpp;OS_NS_sys_select.cpp;OS_NS_sys_sendfile.cpp;OS_NS_sys_shm.cpp;OS_NS_sys_socket.cpp;OS_NS_sys_stat.cpp;OS_NS_sys_time.cpp;OS_NS_sys_uio.cpp;OS_NS_sys_utsname.cpp;OS_NS_sys_wait.cpp;OS_NS_Thread.cpp;OS_NS_time.cpp;OS_NS_unistd.cpp;OS_NS_wchar.cpp;OS_QoS.cpp;OS_Thread_Adapter.cpp;OS_TLI.cpp;Pagefile_Memory_Pool.cpp;Parse_Node.cpp;PI_Malloc.cpp;Ping_Socket.cpp;Pipe.cpp;POSIX_Asynch_IO.cpp;POSIX_CB_Proactor.cpp;POSIX_Proactor.cpp;Priority_Reactor.cpp;Proactor.cpp;Proactor_Impl.cpp;Process.cpp;Process_Manager.cpp;Process_Mutex.cpp;Process_Semaphore.cpp;Profile_Timer.cpp;Reactor.cpp;Reactor_Impl.cpp;Reactor_Notification_Strategy.cpp;Reactor_Timer_Interface.cpp;Read_Buffer.cpp;Recursive_Thread_Mutex.cpp;Recyclable.cpp;Registry.cpp;Registry_Name_Space.cpp;Remote_Name_Space.cpp;Remote_Tokens.cpp;RW_Mutex.cpp;RW_Process_Mutex.cpp;RW_Thread_Mutex.cpp;Sample_History.cpp;Sbrk_Memory_Pool.cpp;Sched_Params.cpp;Select_Reactor_Base.cpp;Semaphore.cpp;Service_Config.cpp;Service_Gestalt.cpp;Service_Manager.cpp;Service_Object.cpp;Service_Repository.cpp;Service_Types.cpp;Shared_Memory.cpp;Shared_Memory_MM.cpp;Shared_Memory_Pool.cpp;Shared_Memory_SV.cpp;Shared_Object.cpp;Sig_Adapter.cpp;Sig_Handler.cpp;Signal.cpp;SOCK.cpp;SOCK_Acceptor.cpp;SOCK_CODgram.cpp;Sock_Connect.cpp;SOCK_Connector.cpp;SOCK_Dgram.cpp;SOCK_Dgram_Bcast.cpp;SOCK_Dgram_Mcast.cpp;SOCK_IO.cpp;SOCK_Netlink.cpp;SOCK_SEQPACK_Acceptor.cpp;SOCK_SEQPACK_Association.cpp;SOCK_SEQPACK_Connector.cpp;SOCK_Stream.cpp;SPIPE.cpp;SPIPE_Acceptor.cpp;SPIPE_Addr.cpp;SPIPE_Connector.cpp;SPIPE_Stream.cpp;SString.cpp;Stack_Trace.cpp;Stats.cpp;String_Base_Const.cpp;SUN_Proactor.cpp;SV_Message.cpp;SV_Message_Queue.cpp;SV_Semaphore_Complex.cpp;SV_Semaphore_Simple.cpp;SV_Shared_Memory.cpp;Svc_Conf_Lexer.cpp;Svc_Conf_y.cpp;Synch_Options.cpp;System_Time.cpp;Task.cpp;Thread.cpp;Thread_Adapter.cpp;Thread_Control.cpp;Thread_Exit.cpp;Thread_Hook.cpp;Thread_Manager.cpp;Thread_Mutex.cpp;Thread_Semaphore.cpp;Throughput_Stats.cpp;Time_Policy.cpp;Time_Value.cpp;Timeprobe.cpp;TLI.cpp;TLI_Acceptor.cpp;TLI_Connector.cpp;TLI_Stream.cpp;Token.cpp;Token_Collection.cpp;Token_Invariants.cpp;Token_Manager.cpp;Token_Request_Reply.cpp;TP_Reactor.cpp;Trace.cpp;TSS_Adapter.cpp;TTY_IO.cpp;UNIX_Addr.cpp;UPIPE_Acceptor.cpp;UPIPE_Connector.cpp;UPIPE_Stream.cpp;UTF16_Encoding_Converter.cpp;UTF32_Encoding_Converter.cpp;UTF8_Encoding_Converter.cpp;UUID.cpp;WFMO_Reactor.cpp;WIN32_Asynch_IO.cpp;WIN32_Proactor.cpp;XML_Svc_Conf.cpp;XTI_ATM_Mcast.cpp")
set (COTIRE_UNITY_BUILD_ALL_TARGET_NAME "all_unity")
set (COTIRE_UNITY_BUILD_TARGET_SUFFIX "_unity")
set (COTIRE_UNITY_OUTPUT_DIRECTORY "")
set (COTIRE_UNITY_SOURCE_EXCLUDE_EXTENSIONS "m;mm")
set (COTIRE_UNITY_SOURCE_FILENAME_SUFFIX "_unity")
set (MSVC "1")
set (CMAKE_GENERATOR "Visual Studio 16 2019")
set (CMAKE_BUILD_TYPE "Release")
set (CMAKE_CONFIGURATION_TYPES "Debug;Release;MinSizeRel;RelWithDebInfo")
set (CMAKE_CXX_COMPILER_ID "MSVC")
set (CMAKE_CXX_COMPILER_VERSION "19.26.28806.0")
set (CMAKE_CXX_COMPILER "C:/Program Files (x86)/Microsoft Visual Studio/2019/Enterprise/VC/Tools/MSVC/14.26.28801/bin/Hostx64/x64/cl.exe")
set (CMAKE_CXX_COMPILER_ARG1 "")
set (CMAKE_INCLUDE_FLAG_CXX "-I")
set (CMAKE_CXX_SOURCE_FILE_EXTENSIONS "C;M;c++;cc;cpp;cxx;mm;CPP")
