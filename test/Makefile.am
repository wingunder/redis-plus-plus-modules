bin_PROGRAMS = test_redismodules

test_redismodules_SOURCES = \
test_main.cpp \
test/test_redisbloom_commands.h \
test/test_redisbloom_commands.tpp \
test/test_bloomfilter_commands.h \
test/test_bloomfilter_commands.tpp \
test/test_countminsketch_commands.h \
test/test_countminsketch_commands.tpp \
test/test_cuckoofilter_commands.h \
test/test_cuckoofilter_commands.tpp \
test/test_topk_commands.h \
test/test_topk_commands.tpp

test_redismodules_CXXFLAGS = -I../include
test_redismodules_LDADD = -L../lib -lredis++ -lhiredis -lpthread
