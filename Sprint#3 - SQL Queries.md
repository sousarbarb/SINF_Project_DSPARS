# Required queries for Sprint#3

***

## Difficulty Level: Easy

* Measurement history of one sensor of type A , which is present in room B ,
within a time duration C
```
SELECT sensor_type, sensor_data AS Measurement, time_update AS Timestamp FROM sensor_history,divisions WHERE sensor_history.id_division = divisions.id  AND sensor_history.sensor_type = 'TEMP' AND divisions.name = 'ROOM1' AND sensor_history.time_update > CURRENT_TIMESTAMP(0) AT TIME ZONE 'GMT-1' - interval '1 hour 30 minutes';
```

* Current state of all actuators that are installed in every room
```
SELECT actuator.name AS Actuator, divisions.name AS Room, actuator.actuator_state AS State FROM actuator, divisions WHERE actuator.id_divisions = divisions.id;
```

* Modify current state of every actuator of type D that exist in every room
```
UPDATE actuator SET actuator_state = 'ON' WHERE name = 'HEAT'; SELECT actuator.name AS Actuator, divisions.name AS Room, actuator.actuator_state AS State FROM actuator, divisions WHERE actuator.name = 'HEAT' AND actuator.id_divisions = divisions.id;
```

* Considering only two sensor motes and two rooms, where each sensor mote is present in a different room, change the location of both sensor motes, so each room will be attributed with the sensor mote that was located in the other room.
```
UPDATE sensor SET id_divisions = CASE mote_id
    WHEN 1 THEN (SELECT DISTINCT id_divisions FROM sensor WHERE mote_id = 2)
    WHEN 2 THEN (SELECT DISTINCT id_divisions FROM sensor WHERE mote_id = 1)
    END
    WHERE mote_id IN (1,2);
SELECT sensor.sensor_type AS Sensor, sensor.mote_id AS Mote_ID, divisions.name AS Room FROM sensor, divisions WHERE (sensor.mote_id=1 OR sensor.mote_id=2) AND sensor.id_divisions = divisions.id;
```

* Modify the reference value of a sensor of type A in a given rule
```
UPDATE rules SET threshold_condition1 = 300 WHERE name_sensor_condition1 = 'LIGHT1';SELECT rules.id AS Rule, divisions.name AS Room, rules.threshold_condition1 AS Reference_Value, rules.name_sensor_condition1 AS Sensor FROM rules, divisions WHERE rules.id_divisions = divisions.id AND rules.name_sensor_condition1 = 'LIGHT1';
```

***

## Difficulty Level: Medium

* Average of all sensors of type A that are present in all rooms, within a time duration C
```
SELECT divisions.name AS Room, CAST(AVG(sensor_history.sensor_data) AS DECIMAL(10,1)) AS Average FROM sensor_history, divisions WHERE sensor_history.id_division = divisions.id AND sensor_history.sensor_type = 'TEMP' AND sensor_history.time_update > CURRENT_TIMESTAMP(0) AT TIME ZONE 'GMT-1' - interval '2 hours' GROUP BY divisions.name;
```

* Number of state changes of every actuator of type D , which is installed in every room, within a time duration C
```
SELECT divisions.name AS Room, COUNT(actuator_history.actuator_state) AS Change FROM actuator_history, divisions WHERE actuator_history.id_division = divisions.id  AND actuator_history.actuator_name = 'WINDOW' AND actuator_history.time_start > CURRENT_TIMESTAMP(0) AT TIME ZONE 'GMT-1' - interval '2 days' GROUP BY divisions.name;
```

* Number of sensors available in every room
```
SELECT divisions.name AS Room, COUNT(DISTINCT sensor.mote_id) AS Mote_Count, COUNT(sensor) AS Sensor_Count FROM divisions, sensor WHERE sensor.id_divisions = divisions.id GROUP BY divisions.name;
```

* Number of rules that exist in every room
```
SELECT divisions.name AS Room, COUNT(rules) AS Rules FROM divisions,rules WHERE rules.id_divisions = divisions.id GROUP BY divisions.name;
```

***

## Difficulty Level: Hard

* Average of all sensors from all sensor types, which are present in every room, within a time duration C
```
SELECT divisions.name AS Room, sensor_history.sensor_type AS Sensor_Type, CAST(AVG(sensor_history.sensor_data) AS DECIMAL(10,1)) AS Average FROM divisions, sensor_history WHERE sensor_history.id_division = divisions.id AND sensor_history.time_update > CURRENT_TIMESTAMP(0) AT TIME ZONE 'GMT-1' - interval '2 days' GROUP BY divisions.name, sensor_history.sensor_type;
```

* Within a time duration of C , actuator identification and the duration of time that the actuator that remained the longest in a state E
```
SELECT actuator_name, actuator_state AS State, SUM(time_final-time_start) AS Duration FROM actuator_history WHERE actuator_state = 'ON' AND time_start > CURRENT_TIMESTAMP(0) AT TIME ZONE 'GMT-1' - interval '2 days' AND (time_final IS DISTINCT FROM NULL) GROUP BY actuator_name, actuator_state ORDER BY SUM(time_final-time_start) DESC LIMIT 1;
```

* Number of sensors and actuators available in every room
```
SELECT divisions.name AS Room, COUNT(DISTINCT actuator.id) AS Actuator, COUNT(DISTINCT sensor.id) AS Sensor FROM sensor, actuator, divisions WHERE sensor.id_divisions = divisions.id AND actuator.id_divisions = divisions.id GROUP BY divisions.name;
```
