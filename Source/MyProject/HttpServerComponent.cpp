
#include "HttpServerComponent.h"

UHttpServerComponent::UHttpServerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHttpServerComponent::BeginPlay()
{
    Super::BeginPlay();

    FHttpServerModule& HttpServerModule = FHttpServerModule::Get();

    HttpRouter = HttpServerModule.GetHttpRouter(ServerPort);
    if (HttpRouter.IsValid())
    {
        FHttpPath Path(TEXT("/hello"));

        TFunction<bool(const FHttpServerRequest&, const FHttpResultCallback&)> Handler =
            [this](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
            {
                return HandleRequest(Request, OnComplete);
            };

        RouteHandle = HttpRouter->BindRoute(Path, EHttpServerRequestVerbs::VERB_GET, Handler);

        HttpServerModule.StartAllListeners();

        UE_LOG(LogTemp, Log, TEXT("HTTP Server started on port %d"), ServerPort);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to get HTTP router"));
    }
}

void UHttpServerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (HttpRouter.IsValid())
    {
        HttpRouter->UnbindRoute(RouteHandle);
        UE_LOG(LogTemp, Log, TEXT("HTTP route unbound"));
    }

    Super::EndPlay(EndPlayReason);
}

bool UHttpServerComponent::HandleRequest(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
{
    FString ResponseText = TEXT("Hello from Unreal HTTP Server!");
    TUniquePtr<FHttpServerResponse> Response = FHttpServerResponse::Create(ResponseText, TEXT("text/plain"));
    OnComplete(MoveTemp(Response));
    return true;
}
