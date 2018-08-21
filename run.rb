#!/usr/bin/env ruby

if __FILE__ == $0
  repository, dir = 'https://github.com/mruby/mruby.git', 'tmp/mruby'

  build_args = ARGV

  Dir.mkdir 'tmp'  unless File.exist?('tmp')
  Dir.mkdir 'tmp/assets'  unless File.exist?('tmp/assets')
  unless File.exist?(dir)
    system "git clone -b 1.4.1 #{repository} #{dir}"
  end

  system "cd #{dir}; MRUBY_CONFIG=#{File.expand_path __FILE__} ruby minirake #{build_args.join(' ')};"

  system "./tmp/mruby/build/host/bin/mrbc -o tmp/assets/sample.mrb sample.rb;"

  FLAGS="-DMRB_32BIT -DMRB_UTF8_STRING -std=gnu11 -Wall -I ./tmp/mruby/include"
  SRC="sample.c"
  system "gcc #{SRC}  -o tmp/sample.exe  -O3 #{FLAGS} -L ./tmp/mruby/build/host/lib -lmruby"
  system "emcc #{SRC} -o tmp/sample.html -Oz #{FLAGS} -L ./tmp/mruby/build/emscripten/lib -lmruby -s WASM=1 --preload-file tmp/assets@assets"
  exit
end

MRuby::Build.new do |conf|
  toolchain :gcc

  conf.gembox 'default'

  conf.cc.defines += %w(MRB_32BIT MRB_UTF8_STRING)
  conf.cc.flags = %w(-O3 -std=gnu11 -Wall -Werror-implicit-function-declaration -Wwrite-strings)
end

MRuby::CrossBuild.new('emscripten') do |conf|
  toolchain :clang

  conf.gembox 'default'

  conf.cc do |cc|
    cc.command = 'emcc'
    cc.defines += %w(MRB_32BIT MRB_UTF8_STRING)
    cc.flags = %W(-g -std=gnu11 -Oz -Wall -Werror-implicit-function-declaration -Wwrite-strings)
  end

  conf.linker.command = 'emcc'
  conf.archiver.command = 'emar'
end
