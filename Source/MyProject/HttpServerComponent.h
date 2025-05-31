#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "HttpServerModule.h"
#include "HttpServerRequest.h"
#include "HttpServerResponse.h"
#include "HttpPath.h"
#include "HttpRouteHandle.h"
#include "IHttpRouter.h"

#include "HttpServerComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UHttpServerComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    UHttpServerComponent();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    bool HandleRequest(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete);

    TSharedPtr<IHttpRouter> HttpRouter;
    FHttpRouteHandle RouteHandle;

    int32 ServerPort = 8080;
};
