-- MySQL dump 10.13  Distrib 8.0.28, for Linux (x86_64)
--
-- Host: localhost    Database: bandu_wangpan
-- ------------------------------------------------------
-- Server version	8.0.28-0ubuntu0.20.04.3

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `File_system`
--

DROP TABLE IF EXISTS `File_system`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `File_system` (
  `file_id` int NOT NULL AUTO_INCREMENT,
  `ppid` varchar(50) DEFAULT NULL,
  `md5sum` varchar(50) DEFAULT NULL,
  `File_name` varchar(50) DEFAULT NULL,
  `belong` varchar(50) DEFAULT NULL,
  `type` varchar(50) DEFAULT NULL,
  `LastName` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`file_id`)
) ENGINE=InnoDB AUTO_INCREMENT=61 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `File_system`
--

LOCK TABLES `File_system` WRITE;
/*!40000 ALTER TABLE `File_system` DISABLE KEYS */;
INSERT INTO `File_system` VALUES (1,'0','0','858626244:~/','858626244','d',NULL),(2,'1','0','my_test','858626244','d',NULL),(3,'1','0','vdieo','858626244','d',NULL),(4,'3','0','book','858626244','d',NULL),(5,'0','0','whx:~/','whx','d',NULL),(6,'5','0','vdieo/','whx','d',NULL),(7,'6','0','book/','whx','d',NULL),(8,'5','0','test2/','whx','d',NULL),(9,'0','','testfile','','f',NULL),(10,'0','','testfile','','f',NULL),(11,'0','0','654321:~/','654321','d',NULL),(12,'11','13e25ecf95732ed835cce074de529b03','2022','654321','f',NULL),(13,'0','0','test:~/','test','d',NULL),(16,'13','0','myFile/','test','d',NULL),(17,'16','676c9ba1752dc649e99723cc092c47f7','test_fil','test','f',NULL),(21,'13','0','shor_test/','test','d',NULL),(22,'0','0','test2:~/','test2','d',NULL),(23,'22','0','testfile/','test2','d',NULL),(24,'23','fb2f37675b582bb2c63d0e7fade660b2','test_myF','test2','f',NULL),(25,'0','t2','d3f8c','','f',NULL),(26,'22','4cba4ac3dc3a2d11abe4add6389d3f8c','file001','test2','f',NULL);
/*!40000 ALTER TABLE `File_system` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `client`
--

DROP TABLE IF EXISTS `client`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `client` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `name` varchar(50) DEFAULT NULL,
  `keyword` varchar(50) DEFAULT NULL,
  `salt` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `client`
--

LOCK TABLES `client` WRITE;
/*!40000 ALTER TABLE `client` DISABLE KEYS */;
INSERT INTO `client` VALUES (1,'1234567','123','45DHOR8NUj'),(2,'858626244','123','24Eq9LDNr9'),(3,'whx','123','7m73Nuza48'),(4,'654321','123','Ca4j86x59e'),(5,'test','123','9GfnRGNg5u'),(6,'test2','123','jZu8AVcFx2');
/*!40000 ALTER TABLE `client` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `md5_count`
--

DROP TABLE IF EXISTS `md5_count`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `md5_count` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `md5sum` varchar(50) DEFAULT NULL,
  `count` varchar(50) DEFAULT NULL,
  `ppid` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=43 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `md5_count`
--

LOCK TABLES `md5_count` WRITE;
/*!40000 ALTER TABLE `md5_count` DISABLE KEYS */;
INSERT INTO `md5_count` VALUES (1,'','1',NULL),(2,'','1',NULL),(3,'13e25ecf95732ed835cce074de529b03','1',NULL),(4,'676c9ba1752dc649e99723cc092c47f7','1',NULL),(6,'fb2f37675b582bb2c63d0e7fade660b2','1',NULL),(7,'t2','1',NULL),(8,'4cba4ac3dc3a2d11abe4add6389d3f8c','1',NULL);
/*!40000 ALTER TABLE `md5_count` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-03-31  4:06:41
