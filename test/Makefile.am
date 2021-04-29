bin_PROGRAMS = test_redisbloom

test_redisbloom_SOURCES = \
test_main.cpp \
test/test_bloombase_commands.h \
test/test_bloombase_commands.tpp \
test/test_bloomfilter_commands.h \
test/test_bloomfilter_commands.tpp \
test/test_countminsketch_commands.h \
test/test_countminsketch_commands.tpp \
test/test_cuckoofilter_commands.h \
test/test_cuckoofilter_commands.tpp \
test/test_topk_commands.h \
test/test_topk_commands.tpp

test_redisbloom_CXXFLAGS = -I../include
test_redisbloom_LDADD = -L../lib -lredis++ -lhiredis -lpthread
