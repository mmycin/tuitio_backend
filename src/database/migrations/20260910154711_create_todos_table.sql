-- Create "todos" table
CREATE TABLE `todos` (
  `id` integer NOT NULL PRIMARY KEY AUTOINCREMENT,
  `user_id` integer NOT NULL,
  `title` text NOT NULL,
  `description` text NOT NULL,
  `completed` bool NOT NULL DEFAULT false,
  CONSTRAINT `todos_user_fk` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON UPDATE CASCADE ON DELETE CASCADE
);
-- Create index "todos_user_id_idx" to table: "todos"
CREATE INDEX `todos_user_id_idx` ON `todos` (`user_id`);
