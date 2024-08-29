-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Aug 29, 2024 at 09:44 AM
-- Server version: 11.5.2-MariaDB
-- PHP Version: 8.3.10

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `Aeroponik`
--

-- --------------------------------------------------------

--
-- Table structure for table `Alarm`
--

CREATE TABLE `Alarm` (
  `id` int(11) NOT NULL,
  `id_box` int(11) NOT NULL,
  `alarm_name` int(11) NOT NULL,
  `if_active` tinyint(1) NOT NULL,
  `trigger` varchar(255) NOT NULL,
  `description` text DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `Box`
--

CREATE TABLE `Box` (
  `box_id` int(11) NOT NULL,
  `box_name` varchar(255) NOT NULL,
  `box_location` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `Digital`
--

CREATE TABLE `Digital` (
  `digital_id` int(11) NOT NULL,
  `box_id` int(11) NOT NULL,
  `digital_name` varchar(255) NOT NULL,
  `val` int(11) NOT NULL,
  `timestamp` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `Sensor`
--

CREATE TABLE `Sensor` (
  `sensor_id` int(11) NOT NULL,
  `box_id` int(11) NOT NULL,
  `sensor_name` varchar(255) NOT NULL,
  `satuan` varchar(255) DEFAULT NULL,
  `besaran` varchar(255) DEFAULT NULL,
  `deskripsi` text DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `Value`
--

CREATE TABLE `Value` (
  `value_id` int(11) NOT NULL,
  `sensor_id` int(11) NOT NULL,
  `val1` int(11) DEFAULT NULL,
  `val2` int(11) DEFAULT NULL,
  `val3` int(11) DEFAULT NULL,
  `val4` int(11) DEFAULT NULL,
  `val5` int(11) DEFAULT NULL,
  `val6` int(11) DEFAULT NULL,
  `timestamp` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `Alarm`
--
ALTER TABLE `Alarm`
  ADD PRIMARY KEY (`id`),
  ADD KEY `box_alarm` (`id_box`);

--
-- Indexes for table `Box`
--
ALTER TABLE `Box`
  ADD PRIMARY KEY (`box_id`);

--
-- Indexes for table `Digital`
--
ALTER TABLE `Digital`
  ADD PRIMARY KEY (`digital_id`),
  ADD KEY `digital_box` (`box_id`);

--
-- Indexes for table `Sensor`
--
ALTER TABLE `Sensor`
  ADD PRIMARY KEY (`sensor_id`),
  ADD KEY `box_sensor` (`box_id`);

--
-- Indexes for table `Value`
--
ALTER TABLE `Value`
  ADD PRIMARY KEY (`value_id`),
  ADD KEY `value_sensor` (`sensor_id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `Alarm`
--
ALTER TABLE `Alarm`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `Box`
--
ALTER TABLE `Box`
  MODIFY `box_id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `Digital`
--
ALTER TABLE `Digital`
  MODIFY `digital_id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `Sensor`
--
ALTER TABLE `Sensor`
  MODIFY `sensor_id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `Value`
--
ALTER TABLE `Value`
  MODIFY `value_id` int(11) NOT NULL AUTO_INCREMENT;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `Alarm`
--
ALTER TABLE `Alarm`
  ADD CONSTRAINT `box_alarm` FOREIGN KEY (`id_box`) REFERENCES `Box` (`box_id`);

--
-- Constraints for table `Digital`
--
ALTER TABLE `Digital`
  ADD CONSTRAINT `digital_box` FOREIGN KEY (`box_id`) REFERENCES `Box` (`box_id`);

--
-- Constraints for table `Sensor`
--
ALTER TABLE `Sensor`
  ADD CONSTRAINT `box_sensor` FOREIGN KEY (`box_id`) REFERENCES `Box` (`box_id`);

--
-- Constraints for table `Value`
--
ALTER TABLE `Value`
  ADD CONSTRAINT `value_sensor` FOREIGN KEY (`sensor_id`) REFERENCES `Sensor` (`sensor_id`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
