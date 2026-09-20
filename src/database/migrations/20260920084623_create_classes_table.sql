-- Create "classes" table
CREATE TABLE `classes` (
  `id` integer NOT NULL PRIMARY KEY AUTOINCREMENT,
  `cycle_id` integer NOT NULL,
  `created_at` datetime NOT NULL,
  `notes` text NOT NULL,
  CONSTRAINT `classes_cycle_fk` FOREIGN KEY (`cycle_id`) REFERENCES `cycles` (`id`) ON UPDATE CASCADE ON DELETE CASCADE
);
-- Create index "classes_cycleid_idx" to table: "classes"
CREATE INDEX `classes_cycleid_idx` ON `classes` (`cycle_id`);
