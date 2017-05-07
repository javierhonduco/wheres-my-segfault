filename_line="${1:?}"

echo "==> offending sigsegv line"
python -c "import linecache; filename, line = '$filename_line'.split(':'); print(linecache.getline(filename, int(line)).strip())"
