import sys                                                                                                                   
sys.path.insert(0, 'scripts')                                                                                                
from orca_profile_tool import generate_preset_setting_id as quickgen                                                         

'''
vendor = input("Who's your vendor? Type it in: ")
ptype = input("Filament? Machine? Process? Type it in: ")
fullname = input("DON'T MAKE A TYPO! Name your new profile: ")
'''

print("CAUTION! Do not make a typo. You cannot go back once done.")
print("Vendor: ")
vendor = sys.stdin.readline().strip('\n')
print("\nProfile type: ")
ptype = sys.stdin.readline().strip('\n')
print("\nName: ")
pname = sys.stdin.readline().strip('\n')
print()
print(vendor, ptype, pname)
print("\"setting_id\": \"%s\"" % (quickgen(vendor, ptype, pname)))                                                           
