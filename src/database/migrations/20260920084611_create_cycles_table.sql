-- Create "cycles" table
CREATE TABLE `cycles` (
  `id` integer NOT NULL PRIMARY KEY AUTOINCREMENT,
  `student_id` integer NOT NULL,
  `started_at` datetime NOT NULL,
  `class_count` integer NOT NULL DEFAULT 0,
  `is_paid` bool NOT NULL DEFAULT false,
  CONSTRAINT `cycles_student_fk` FOREIGN KEY (`student_id`) REFERENCES `students` (`id`) ON UPDATE CASCADE ON DELETE CASCADE
);
-- Create index "cycles_student_id_idx" to table: "cycles"
CREATE INDEX `cycles_student_id_idx` ON `cycles` (`student_id`);
