-- --------------------------------------------------------------------------------------
-- Profession NPC
-- --------------------------------------------------------------------------------------

DELETE FROM `creature_template` WHERE `entry`= 550010;
INSERT INTO `creature_template` VALUES
(550010, 0, 0, 0, 0, 0, 29988, 0, 0, 0, 'Lyng the Drunk', 'Professions Trainer', 'Speak', 0, 80, 80, 0, 35, 1, 1, 1.14286, 0.25, 1, 1000, 3000, 0, 0, 15, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 10, 1, 1, 0, 0, 1, 0, 0, 'CreatureScript_Professions', 0);