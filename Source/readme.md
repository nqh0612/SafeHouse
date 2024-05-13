modules within interrupt:
module 1 - Duy_DAC(PWM)_14 (dung timer1)       
module 2 - Huy_button_8led7_18 (dung timer0)  
module 3 - TanLoc_Thoigianthuc_9 (dung timer0) 
module 4 - Son_PWM-DC_17 (dung timer1) 
module 5 - Tai_Timer_8 - ok
module 6 - Tam_buzzer_19

modules without interrupt:
module 7 - Huy_LCD12864_16 
module 8 - Huy_LED7doan_3 
module 9 - Kha_MotorULN2003_06 
module 10 - Liem_LCD1602_7
module 11 - Thuc_LED_MATRIX_20  (Tri_15 the same module 11)  
module 12 - Dat_LED+Button(Polling)_1_2 
	
implement modules (within interrupt):
module 13 - Quy_EEPROM24_13 (dung timer0) (only run on actual kit, not simulation kit)
module 14 - QuangLoc_DS18B20_11

-> integration module: super_module

sử dụng compiler KeilC
