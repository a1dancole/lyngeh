/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
DROP TABLE IF EXISTS `disenchant_loot_template`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `disenchant_loot_template` 
(
  `Entry` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `Item` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `Reference` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `Chance` float NOT NULL DEFAULT '100',
  `QuestRequired` tinyint(1) NOT NULL DEFAULT '0',
  `LootMode` smallint(5) unsigned NOT NULL DEFAULT '1',
  `GroupId` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MinCount` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `MaxCount` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `Comment` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`Entry`,`Item`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Loot System';
/*!40101 SET character_set_client = @saved_cs_client */;

LOCK TABLES `disenchant_loot_template` WRITE;
/*!40000 ALTER TABLE `disenchant_loot_template` DISABLE KEYS */;
INSERT INTO `disenchant_loot_template` VALUES 
(1,10938,0,0,0,1,1,1,2,NULL),
(1,10940,0,80,0,1,1,1,2,NULL),
(2,10939,0,20,0,1,1,1,2,NULL),
(2,10940,0,75,0,1,1,2,3,NULL),
(2,10978,0,0,0,1,1,1,1,NULL),
(3,10940,0,75,0,1,1,4,6,NULL),
(3,10978,0,0,0,1,1,1,1,NULL),
(3,10998,0,15,0,1,1,1,2,NULL),
(4,11082,0,20,0,1,1,1,2,NULL),
(4,11083,0,75,0,1,1,1,2,NULL),
(4,11084,0,0,0,1,1,1,1,NULL),
(5,11083,0,75,0,1,1,2,5,NULL),
(5,11134,0,20,0,1,1,1,2,NULL),
(5,11138,0,0,0,1,1,1,1,NULL),
(6,11135,0,20,0,1,1,1,2,NULL),
(6,11137,0,75,0,1,1,1,2,NULL),
(6,11139,0,0,0,1,1,1,1,NULL),
(7,11137,0,75,0,1,1,2,5,NULL),
(7,11174,0,20,0,1,1,1,2,NULL),
(7,11177,0,0,0,1,1,1,1,NULL),
(8,11175,0,20,0,1,1,1,2,NULL),
(8,11176,0,75,0,1,1,1,2,NULL),
(8,11178,0,0,0,1,1,1,1,NULL),
(9,11176,0,75,0,1,1,2,5,NULL),
(9,14343,0,0,0,1,1,1,1,NULL),
(9,16202,0,20,0,1,1,1,2,NULL),
(10,14344,0,0,0,1,1,1,1,NULL),
(10,16203,0,20,0,1,1,1,2,NULL),
(10,16204,0,75,0,1,1,1,2,NULL),
(11,14344,0,0,0,1,1,1,1,NULL),
(11,16203,0,20,0,1,1,2,3,NULL),
(11,16204,0,75,0,1,1,2,5,NULL),
(12,22445,0,75,0,1,1,1,3,NULL),
(12,22447,0,22,0,1,1,1,3,NULL),
(12,22448,0,0,0,1,1,1,1,NULL),
(13,22445,0,75,0,1,1,2,3,NULL),
(13,22447,0,22,0,1,1,2,3,NULL),
(13,22448,0,0,0,1,1,1,1,NULL),
(14,22445,0,75,0,1,1,2,5,NULL),
(14,22446,0,22,0,1,1,1,2,NULL),
(14,22449,0,0,0,1,1,1,1,NULL),
(15,34053,0,0,0,1,1,1,1,NULL),
(15,34054,0,75,0,1,1,1,2,NULL),
(15,34056,0,22,0,1,1,1,2,NULL),
(16,34052,0,0,0,1,1,1,1,NULL),
(16,34054,0,75,0,1,1,2,5,NULL),
(16,34055,0,22,0,1,1,1,2,NULL),
(17,11139,0,100,0,1,0,2,4,NULL),
(18,14344,0,75,0,1,0,1,1,NULL),
(21,10938,0,80,0,1,1,1,2,NULL),
(21,10940,0,0,0,1,1,1,2,NULL),
(22,10939,0,75,0,1,1,1,2,NULL),
(22,10940,0,20,0,1,1,2,3,NULL),
(22,10978,0,0,0,1,1,1,1,NULL),
(23,10940,0,15,0,1,1,4,6,NULL),
(23,10978,0,0,0,1,1,1,1,NULL),
(23,10998,0,75,0,1,1,1,2,NULL),
(24,11082,0,75,0,1,1,1,2,NULL),
(24,11083,0,20,0,1,1,1,2,NULL),
(24,11084,0,0,0,1,1,1,1,NULL),
(25,11083,0,20,0,1,1,2,5,NULL),
(25,11134,0,75,0,1,1,1,2,NULL),
(25,11138,0,0,0,1,1,1,1,NULL),
(26,11135,0,75,0,1,1,1,2,NULL),
(26,11137,0,20,0,1,1,1,2,NULL),
(26,11139,0,0,0,1,1,1,1,NULL),
(27,11137,0,20,0,1,1,2,5,NULL),
(27,11174,0,75,0,1,1,1,2,NULL),
(27,11177,0,0,0,1,1,1,1,NULL),
(28,11175,0,75,0,1,1,1,2,NULL),
(28,11176,0,20,0,1,1,1,2,NULL),
(28,11178,0,0,0,1,1,1,1,NULL),
(29,11176,0,22,0,1,1,2,5,NULL),
(29,14343,0,0,0,1,1,1,1,NULL),
(29,16202,0,75,0,1,1,1,2,NULL),
(30,14344,0,0,0,1,1,1,1,NULL),
(30,16203,0,75,0,1,1,1,2,NULL),
(30,16204,0,22,0,1,1,1,2,NULL),
(31,14344,0,0,0,1,1,1,1,NULL),
(31,16203,0,75,0,1,1,2,3,NULL),
(31,16204,0,22,0,1,1,2,5,NULL),
(32,22445,0,22,0,1,1,2,3,NULL),
(32,22447,0,75,0,1,1,2,3,NULL),
(32,22448,0,0,0,1,1,1,1,NULL),
(33,22445,0,22,0,1,1,2,5,NULL),
(33,22446,0,75,0,1,1,1,2,NULL),
(33,22449,0,0,0,1,1,1,1,NULL),
(34,34053,0,0,0,1,1,1,1,NULL),
(34,34054,0,22,0,1,1,1,2,NULL),
(34,34056,0,75,0,1,1,1,2,NULL),
(35,34052,0,0,0,1,1,1,1,NULL),
(35,34054,0,22,0,1,1,2,5,NULL),
(35,34055,0,75,0,1,1,1,2,NULL),
(41,10978,0,100,0,1,0,1,1,NULL),
(42,11084,0,100,0,1,0,1,1,NULL),
(43,11138,0,100,0,1,0,1,1,NULL),
(44,11139,0,100,0,1,0,1,1,NULL),
(45,11177,0,100,0,1,0,1,1,NULL),
(46,11178,0,100,0,1,0,1,1,NULL),
(47,14343,0,100,0,1,0,1,1,NULL),
(48,14344,0,99.5,0,1,1,1,1,NULL),
(48,20725,0,0,0,1,1,1,1,NULL),
(49,14344,0,99.5,0,1,1,1,1,NULL),
(49,20725,0,0,0,1,1,1,1,NULL),
(50,20725,0,0,0,1,1,1,1,NULL),
(50,22448,0,99.5,0,1,1,1,1,NULL),
(51,20725,0,0,0,1,1,1,1,NULL),
(51,22448,0,99.5,0,1,1,1,1,NULL),
(52,22449,0,99.5,0,1,1,1,1,NULL),
(52,22450,0,0,0,1,1,1,1,NULL),
(53,34053,0,99.5,0,1,1,1,1,NULL),
(53,34057,0,0,0,1,1,1,1,NULL),
(54,34052,0,99.5,0,1,1,1,1,NULL),
(54,34057,0,0,0,1,1,1,1,NULL),
(61,11177,0,100,0,1,0,2,4,NULL),
(62,11178,0,100,0,1,0,2,4,NULL),
(63,14343,0,100,0,1,0,2,4,NULL),
(64,20725,0,100,0,1,0,1,1,NULL),
(65,20725,0,100,0,1,0,1,2,NULL),
(66,22450,0,100,0,1,0,1,2,NULL),
(67,22450,0,100,0,1,0,1,1,NULL),
(67,44012,44012,67,0,1,1,1,1,NULL),
(68,34057,0,100,0,1,0,1,1,NULL),
(69,34057,0,100,0,1,0,1,2,NULL);
/*!40000 ALTER TABLE `disenchant_loot_template` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

