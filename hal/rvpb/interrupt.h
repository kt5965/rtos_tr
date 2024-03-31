#ifndef HAL_RVPB_INTERRUPT_H_
#define HAL_RVPB_INTERRUPT_H_

typedef union CpuControl_t
{
    uint32_t all;
    struct {
        uint32_t Enable:1;
        uint32_t reserved:31;
    } bits;
} CpuControl_t;

typedef union PriorityMask_t
{
    uint32_t all;
    struct {
        uint32_t Reserved:4;
        uint32_t Prioritymask:4;
        uint32_t reserved:24;
    } bits;
} PriorityMask_t;

typedef union BinaryPoint_t
{
    uint32_t all;
    struct {
        uint32_t Binarypoint:3;
        uint32_t reserved:29;
    } bits;
} BinaryPoint_t;

typedef union InterruptAck_t
{
    uint32_t all;
    struct {
        uint32_t InterruptID:10;
        uint32_t CPUsourceID:3;
        uint32_t reserved:19;
    } bits;
} InterruptAck_t;

typedef union EndOfInterrupt_t
{
    uint32_t all;
    struct {
        uint32_t InterruptID:10;
        uint32_t CPUsourceID:3;
        uint32_t reserved:19;
    } bits;
} EndOfInterrupt_t;

typedef union RunningInterrupt_t
{
    uint32_t all;
    struct {
        uint32_t Reserved:4;
        uint32_t Priority:4;
        uint32_t reserved:24;
    } bits;
} RunningInterrupt_t;

typedef union HighestPendInter_t
{
    uint32_t all;
    struct {
        uint32_t InterruptID:10;
        uint32_t CPUsourceID:3;
        uint32_t reserved:19;
    } bits;
} HighestPendInter_t;

typedef union DistributorCtrl_t
{
    uint32_t all;
    struct {
        uint32_t Enable:1;
        uint32_t reserved:31;
    } bits;
} DistributorCtrl_t;

typedef union ControllerType_t
{
    uint32_t all;
    struct {
        uint32_t IDlinesnumber:5;
        uint32_t CPUnumber:3;
        uint32_t reserved:24;
    } bits;
} ControllerType_t;



typedef struct GicCput_t
{
    CpuControl_t       cpucontrol;
    PriorityMask_t     prioritymask;
    BinaryPoint_t      binarypoint;
    InterruptAck_t     interruptack;
    EndOfInterrupt_t   endofinterrupt;
    RunningInterrupt_t runninginterrupt;
    HighestPendInter_t highestpendinter;
} GicCput_t;

typedef struct GicDist_t
{
    DistributorCtrl_t   distributorctrl;
    ControllerType_t    controllertype;
    uint32_t            reserved0[62];
    uint32_t            reserved1;
    uint32_t            setenable1;
    uint32_t            setenable2;
    uint32_t            reserved2[29];
    uint32_t            reserved3;
    uint32_t            clearenable1;
    uint32_t            clearenable2;
} GicDist_t;

#define GIC_CPU_BASE  0x1E000000
#define GIC_DIST_BASE 0x1E001000

#define GIC_PRIORITY_MASK_NONE  0xF // 모든 인터럽트 허용

#define GIC_IRQ_START           32
#define GIC_IRQ_END             95

#endif