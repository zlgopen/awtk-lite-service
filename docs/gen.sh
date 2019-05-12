node ../../awtk/tools/idl_gen/index.js idl.json ../src/
node ../../awtk/tools/doc_gen/index.js idl.json
rm -rf manual
mv docs manual
