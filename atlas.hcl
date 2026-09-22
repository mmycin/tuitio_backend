env "local" {
  src = "file://src/database/schemas/"
  dev = "sqlite://src/database/migrations/dev.db"
  url = "sqlite://./${getenv("DB_FILENAME")}"

  migration {
    dir = "file://src/database/migrations"
  }
}
