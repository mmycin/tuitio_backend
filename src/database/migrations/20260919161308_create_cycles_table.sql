-- Create "users" table
CREATE TABLE `users` (
  `id` integer NOT NULL PRIMARY KEY AUTOINCREMENT,
  `name` text NOT NULL,
  `email` text NOT NULL,
  `password_hash` text NOT NULL
);
-- Create index "users_email_unique" to table: "users"
CREATE UNIQUE INDEX `users_email_unique` ON `users` (`email`);
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
