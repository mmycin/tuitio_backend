table "payments" {
	schema = schema.main

	column "id" {
		type = integer
		null = false
		auto_increment = true
	}

	column "student_id" {
		type = integer
		null = false
	}

	column "created_at" {
		type = datetime
		null = false
	}

	column "amount" {
		type = integer
		null = false
	}


	primary_key {
		columns = [column.id]
	}

	foreign_key "payments_student_fk" {
		columns = [column.student_id]
		ref_columns = [table.students.column.id]

		on_update = CASCADE
		on_delete = CASCADE
	}

	index "payments_student_id_idx" {
		columns = [column.student_id]
	}
}