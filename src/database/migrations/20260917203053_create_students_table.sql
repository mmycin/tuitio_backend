-- Create "students" table
CREATE TABLE `students` (
  `id` integer NOT NULL PRIMARY KEY AUTOINCREMENT,
  `user_id` integer NOT NULL,
  `name` text NOT NULL,
  `fee` integer NOT NULL,
  CONSTRAINT `students_user_fk` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON UPDATE CASCADE ON DELETE CASCADE
);
-- Create index "students_user_id_idx" to table: "students"
CREATE INDEX `students_user_id_idx` ON `students` (`user_id`);
