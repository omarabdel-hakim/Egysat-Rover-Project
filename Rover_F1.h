void IR_init(void);
void IR_start(void);

void GAS_init(void);
void GAS_start(void);

void UlTRASONIC1_init(void);
unsigned char UlTRASONIC1_start();

void UlTRASONIC2_init(void);
unsigned char UlTRASONIC2_start();

void DHT_start(void);

void MOTORS_init(void);
void MOTORS_forward(void);
void MOTORS_backward(void);
void MOTORS_rotate_right(void);
void MOTORS_rotate_left(void);
void MOTORS_stop(void);

void Collect_TEM( void);
void Rover_init(void);
void Start_scan(void);
