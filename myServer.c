#include "open62541.h"
#include <signal.h>
#include <stdlib.h>

static volatile UA_Boolean running = true;
static void stopHandler(int sig) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    running = false;
}


int main(int argc, char * argv[]) {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    UA_Server *server = UA_Server_new();
    UA_ServerConfig_setDefault(UA_Server_getConfig(server));

    //chekc for arguments
    if (argc > 2) {  //hostname or ip address and a port number are available
        UA_Int16 port_number = atoi(argv[2]);
        UA_ServerConfig_setMinimal(UA_Server_getConfig(server), port_number, 0);
    }
    else
        UA_ServerConfig_setDefault(UA_Server_getConfig(server));

    if (argc > 1) {  //hostname or ip address available
        //Copy the hostname from char * to an open62541 variable
        UA_String hostname;
        UA_String_init(&hostname);
        hostname.length = strlen(argv[1]);
        hostname.data = (UA_Byte*)argv[1];

        //change the configuration
        UA_ServerConfig_setCustomHostname(UA_Server_getConfig(server), hostname);
    }

    //Add a new namespace to the server
    UA_Int16 ns_room1 = UA_Server_addNamespace(server, "Room1");
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "New Namespace added with Nr. %d", ns_room1);

    //Add a new object called Tempreture Sensor
    UA_NodeId r1_tempsens_Id;
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    UA_Server_addObjectNode(server, UA_NODEID_STRING(2, "TempretureSensor"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
        UA_QUALIFIEDNAME(2, "Tempreture Sensor"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
        oAttr, NULL, &r1_tempsens_Id);

    //Add the variables vendor name to server
    UA_VariableAttributes vnAttr = UA_VariableAttributes_default;
    UA_String vendorName = UA_STRING("Sensor King Ltd.");
    UA_Variant_setScalar(&vnAttr.value, &vendorName, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, "R1_TS1_VendorName"), r1_tempsens_Id,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(2, "VendorName"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), vnAttr, NULL, NULL);

    //Add the variables serial number to server
    UA_VariableAttributes snAttr = UA_VariableAttributes_default;
    UA_Int32 serialNumber = 123456789;
    UA_Variant_setScalar(&snAttr.value, &serialNumber, &UA_TYPES[UA_TYPES_INT32]);
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, "R1_TS1_SerialNumber"), r1_tempsens_Id,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(2, "SerialNumber"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), snAttr, NULL, NULL);

    //Add the variables tempreture to server
    UA_VariableAttributes tpAttr = UA_VariableAttributes_default;
    UA_Int32 Tempreture = 20;
    UA_Variant_setScalar(&tpAttr.value, &Tempreture, &UA_TYPES[DEVPROP_TYPE_INT32]);
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, "R1_TS1_Tempreture"), r1_tempsens_Id,
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
        UA_QUALIFIEDNAME(2, "Tempreture"),
        UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), tpAttr, NULL, NULL);


    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Starting server...");
    UA_StatusCode retval = UA_Server_run(server, &running);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Server was shut down");

    UA_Server_delete(server);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}