#!/bin/sh

create_auto_login() {
	# Define the content of auto_login.sh
	AUTO_LOGIN_CONTENT="#!/bin/sh\nexec /bin/login -f root"

	# Define the target file path
	TARGET_FILE="${TARGET_DIR}/bin/auto_login.sh"

	# Create or overwrite the file with the content
	echo "$AUTO_LOGIN_CONTENT" > $TARGET_FILE

	# Set executable permissions for the script
	chmod +x $TARGET_FILE
}

# Call the function
create_auto_login
