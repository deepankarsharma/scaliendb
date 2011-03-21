#include "ShardServerApp.h"
#include "System/Config.h"
#include "Application/Common/ContextTransport.h"

void ShardServerApp::Init()
{
    shardServer.Init();
    
    httpContext.SetShardServer(&shardServer);
    httpServer.Init(configFile.GetIntValue("http.port", 8080));
    httpServer.RegisterHandler(&httpContext);
  
    sdbpServer.Init(configFile.GetIntValue("sdbp.port", 7080));
    sdbpServer.SetContext(&shardServer);

    shardServer.GetDatabaseManager()->GetEnvironment()->SetMergeEnabled(
     configFile.GetBoolValue("database.merge", true));
}

void ShardServerApp::Shutdown()
{
    sdbpServer.Shutdown();
    httpServer.Shutdown();
    shardServer.Shutdown();
}
