@ Array of per-channel telemetry counters
@ Each element contains receive counters, sent counters, fault counters,
@ queue depths, and activity counters for one CFDP channel.
telemetry ChannelTelemetry: Cfdp.ChannelTelemetryArray

@ Each channel's dequeue state and the Enable and Disable commands it ran since boot
telemetry Dequeue: Cfdp.DequeueTlmArray
