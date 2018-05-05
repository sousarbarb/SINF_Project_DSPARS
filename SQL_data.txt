INSERT INTO building (address, num_floors, owner, num_apartments) VALUES ('Rua Dr. Roberto Frias, 4200-465 Porto', 1, 'Daniel, Pedro e Ricardo Lda.', 1);

INSERT INTO apartment (id, floor, number, proprietary, num_divisions, address_building) VALUES (0,0,1,'Armando Luís Sousa Araújo',4,'Rua Dr. Roberto Frias, 4200-465 Porto');

INSERT INTO divisions (id,name,num_sensors,num_actuators,id_apartment,num_rules) VALUES (0,'ROOM1',2,3,0,4);
INSERT INTO divisions (id,name,num_sensors,num_actuators,id_apartment,num_rules) VALUES (1,'ROOM2',3,3,0,2);
INSERT INTO divisions (id,name,num_sensors,num_actuators,id_apartment,num_rules) VALUES (2,'LIVING_ROOM',2,1,0,2);
INSERT INTO divisions (id,name,num_sensors,num_actuators,id_apartment,num_rules) VALUES (3,'KITCHEN',2,3,0,2);

INSERT INTO actuator (id,name,actuator_state,time,id_divisions,day, act_matrix_id) values (0,'LIGHT','OFF',CURRENT_TIME(0) AT TIME ZONE 'GMT-1',0,CURRENT_DATE,0);
INSERT INTO actuator (id,name,actuator_state,time,id_divisions,day, act_matrix_id) values (1,'BLINDS','OFF',CURRENT_TIME(0) AT TIME ZONE 'GMT-1',0,CURRENT_DATE,1);
INSERT INTO actuator (id,name,actuator_state,time,id_divisions,day, act_matrix_id) values (2,'HEAT','OFF',CURRENT_TIME(0) AT TIME ZONE 'GMT-1',0,CURRENT_DATE,2);
INSERT INTO actuator (id,name,actuator_state,time,id_divisions,day, act_matrix_id) values (3,'WINDOW','OFF',CURRENT_TIME(0) AT TIME ZONE 'GMT-1',1,CURRENT_DATE,0);
INSERT INTO actuator (id,name,actuator_state,time,id_divisions,day, act_matrix_id) values (4,'HEAT','OFF',CURRENT_TIME(0) AT TIME ZONE 'GMT-1',1,CURRENT_DATE, 1);
INSERT INTO actuator (id,name,actuator_state,time,id_divisions,day, act_matrix_id) values (5,'WINDOW','OFF',CURRENT_TIME(0) AT TIME ZONE 'GMT-1',2,CURRENT_DATE, 0);
INSERT INTO actuator (id,name,actuator_state,time,id_divisions,day, act_matrix_id) values (6,'WINDOW','OFF',CURRENT_TIME(0) AT TIME ZONE 'GMT-1',3,CURRENT_DATE, 0);
INSERT INTO actuator (id,name,actuator_state,time,id_divisions,day, act_matrix_id) values (7,'HEAT','OFF',CURRENT_TIME(0) AT TIME ZONE 'GMT-1',3,CURRENT_DATE,1);
INSERT INTO actuator (id,name,actuator_state,time,id_divisions,day, act_matrix_id) values (8,'BLINDS','OFF',CURRENT_TIME(0) AT TIME ZONE 'GMT-1',3,CURRENT_DATE,2);

INSERT INTO sensor (id,mote_id,sensor_type,data,time,id_divisions,day,sens_matrix_id) values (0,1,'TEMP',0,CURRENT_TIME(0) AT TIME ZONE 'GMT-1',0,CURRENT_DATE, 0);
INSERT INTO sensor (id,mote_id,sensor_type,data,time,id_divisions,day,sens_matrix_id) values (1,1,'LIGHT',0,CURRENT_TIME(0) AT TIME ZONE 'GMT-1',0,CURRENT_DATE, 1);
INSERT INTO sensor (id,mote_id,sensor_type,data,time,id_divisions,day,sens_matrix_id) values (2,2,'TEMP',0,CURRENT_TIME(0) AT TIME ZONE 'GMT-1',1,CURRENT_DATE, 0);
INSERT INTO sensor (id,mote_id,sensor_type,data,time,id_divisions,day,sens_matrix_id) values (3,3,'TEMP',0,CURRENT_TIME(0) AT TIME ZONE 'GMT-1',2,CURRENT_DATE, 0);
INSERT INTO sensor (id,mote_id,sensor_type,data,time,id_divisions,day,sens_matrix_id) values (4,3,'HUM',0,CURRENT_TIME(0) AT TIME ZONE 'GMT-1',2,CURRENT_DATE, 1);
INSERT INTO sensor (id,mote_id,sensor_type,data,time,id_divisions,day,sens_matrix_id) values (5,4,'TEMP',0,CURRENT_TIME(0) AT TIME ZONE 'GMT-1',3,CURRENT_DATE, 0);
INSERT INTO sensor (id,mote_id,sensor_type,data,time,id_divisions,day,sens_matrix_id) values (6,4,'HUM',0,CURRENT_TIME(0) AT TIME ZONE 'GMT-1',3,CURRENT_DATE, 1);
