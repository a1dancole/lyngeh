-- DB update 2020_04_24_00 -> 2020_04_25_00
DROP PROCEDURE IF EXISTS `updateDb`;
DELIMITER //
CREATE PROCEDURE updateDb ()
proc:BEGIN DECLARE OK VARCHAR(100) DEFAULT 'FALSE';
SELECT COUNT(*) INTO @COLEXISTS
FROM information_schema.COLUMNS
WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'version_db_world' AND COLUMN_NAME = '2020_04_24_00';
IF @COLEXISTS = 0 THEN LEAVE proc; END IF;
START TRANSACTION;
ALTER TABLE version_db_world CHANGE COLUMN 2020_04_24_00 2020_04_25_00 bit;
SELECT sql_rev INTO OK FROM version_db_world WHERE sql_rev = '1586136135568974600'; IF OK <> 'FALSE' THEN LEAVE proc; END IF;
--
-- START UPDATING QUERIES
--

INSERT INTO `version_db_world` (`sql_rev`) VALUES ('1586136135568974600');

SET @OBJ := 188086;
SET @GUID := @OBJ * 10;
DELETE FROM `gameobject` WHERE `guid` = 268597 AND `id` = 188086;
DELETE FROM `gameobject` WHERE `guid` IN (@GUID+0,@GUID+1,@GUID+2,@GUID+3,@GUID+4,@GUID+5,@GUID+6,@GUID+7,@GUID+8,@GUID+9,@GUID+10,@GUID+11,@GUID+12,@GUID+13,@GUID+14,@GUID+15,@GUID+16,@GUID+17,@GUID+18,@GUID+19);
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `ScriptName`, `VerifiedBuild`)
VALUES 
(@GUID+0, 188086, 571, 0, 0, 1, 1, 4043.9, 5256.3, 26.1286, 2.36773, 0, 0, -0.926071, -0.37735, 300, 0, 1, '', 0),
(@GUID+1, 188086, 571, 0, 0, 1, 1, 4319.64, 5122.84, 23.27, 0.376741, 0, 0, -0.187258, -0.982311, 300, 0, 1, '', 0),
(@GUID+2, 188086, 571, 0, 0, 1, 1, 4169.85, 5081.27, 15.46, 1.37812, 0, 0, -0.635814, -0.771843, 300, 0, 1, '', 0),
(@GUID+3, 188086, 571, 0, 0, 1, 1, 3931.93, 5029.23, -13.67, 1.18491, 0, 0, -0.558401, -0.829571, 300, 0, 1, '', 0),
(@GUID+4, 188086, 571, 0, 0, 1, 1, 3995.50, 4952.19, -7.01, 5.55959, 0, 0, -0.353955, 0.935262, 300, 0, 1, '', 0),
(@GUID+5, 188086, 571, 0, 0, 1, 1, 3660.94, 4681.66, -13.67, 4.49537, 0, 0, -0.779525, 0.626371, 300, 0, 1, '', 0),
(@GUID+6, 188086, 571, 0, 0, 1, 1, 3632.99, 4665.46, -13.0169, 4.19692, 0, 0, -0.863985, 0.503518, 300, 0, 1, '', 0),
(@GUID+7, 188086, 571, 0, 0, 1, 1, 3762.16, 4780.80, 13.31, 0.792213, 0, 0, -0.385829, -0.92257, 300, 0, 1, '', 0),
(@GUID+8, 188086, 571, 0, 0, 1, 1, 3618, 4882.79, 3.68085, 5.22186, 0, 0, -0.506104, 0.862472, 300, 0, 1, '', 0),
(@GUID+9, 188086, 571, 0, 0, 1, 1, 3663.03, 4636.13, -13.67, 1.66401, 0, 0, -0.739285, -0.673393, 300, 0, 1, '', 0),
(@GUID+10, 188086, 571, 0, 0, 1, 1, 3563.72, 4609.3, -5.21892, 5.5478, 0, 0, -0.359464, 0.933159, 300, 0, 1, '', 0),
(@GUID+11, 188086, 571, 0, 0, 1, 1, 3723.74, 4739.80, -13.90, 5.21008, 0, 0, -0.511177, 0.859475, 300, 0, 1, '', 0),
(@GUID+12, 188086, 571, 0, 0, 1, 1, 3760.99, 4781.12, 13.01, 5.37502, 0, 0, -0.438636, 0.898665, 300, 0, 1, '', 0),
(@GUID+13, 188086, 571, 0, 0, 1, 1, 4011.28, 4995.70, -4.86, 5.80699, 0, 0, -0.235855, 0.971788, 300, 0, 1, '', 0),
(@GUID+14, 188086, 571, 0, 0, 1, 1, 4175.71, 4804.57, -10.23, 5.41822, 0, 0, -0.419128, 0.907927, 300, 0, 1, '', 0),
(@GUID+15, 188086, 571, 0, 0, 1, 1, 4088.39, 4925.47, -11.01, 6.07794, 0, 0, -0.10244, 0.994739, 300, 0, 1, '', 0),
(@GUID+16, 188086, 571, 0, 0, 1, 1, 4205.54, 5110.98, 15.95, 0.502405, 0, 0, -0.248569, -0.968614, 300, 0, 1, '', 0),
(@GUID+17, 188086, 571, 0, 0, 1, 1, 3847.79, 4882.8, -2.85, 2.09205, 0, 0, -0.865438, -0.501016, 300, 0, 1, '', 0),
(@GUID+18, 188086, 571, 0, 0, 1, 1, 3911.09, 4971.11, -8.0766, 1.10245, 0, 0, -0.52373, -0.851885, 300, 0, 1, '', 0),
(@GUID+19, 188086, 571, 0, 0, 1, 1, 3883.16, 4797.46, -4.31657, 3.32512, 0, 0, -0.995792, 0.0916371, 300, 0, 1, '', 0);

--
-- END UPDATING QUERIES
--
COMMIT;
END //
DELIMITER ;
CALL updateDb();
DROP PROCEDURE IF EXISTS `updateDb`;
