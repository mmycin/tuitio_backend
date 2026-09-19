-- Create "payments" table
CREATE TABLE `payments` (
  `id` integer NOT NULL PRIMARY KEY AUTOINCREMENT,
  `student_id` integer NOT NULL,
  `created_at` datetime NOT NULL,
  `amount` integer NOT NULL,
  CONSTRAINT `payments_student_fk` FOREIGN KEY (`student_id`) REFERENCES `students` (`id`) ON UPDATE CASCADE ON DELETE CASCADE
);
-- Create index "payments_student_id_idx" to table: "payments"
CREATE INDEX `payments_student_id_idx` ON `payments` (`student_id`);
