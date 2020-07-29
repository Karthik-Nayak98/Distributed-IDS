#! /usr/bin/awk -f 

/Failed/{
	for (i=1; i<=NF; ++i)
	{ 
		if ($i ~ /[0-9]*\.[0-9]*\.[0-9]*\.[0-9]*/) 

			print $i, $1, $2;
	}
	
}
