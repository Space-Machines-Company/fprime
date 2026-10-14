@ Let a channel make pending transactions active
async command EnableDequeue(
    channelId: U8 @< Channel ID
)

@ Hold a channel's pending transactions; active ones run to their end
async command DisableDequeue(
    channelId: U8 @< Channel ID
)
