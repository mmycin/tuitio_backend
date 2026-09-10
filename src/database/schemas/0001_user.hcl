table "users" {
	schema = schema.main

	column "id" {
		type = integer
		null = false
		auto_increment = true
	}

	column "name" {
		type = text
		null = false
	}

	column "email" {
		type = text
		null = false
	}

	column "password_hash" {
		type = text
		null = false
	}

	primary_key {
		columns = [column.id]
	}

	index "users_email_unique" {
		unique = true
		columns = [column.email]
	}
}