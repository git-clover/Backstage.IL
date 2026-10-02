import sys                                                                                                                   
sys.path.insert(0, 'scripts')                                                                                                
from orca_profile_tool import generate_preset_setting_id as quickgen                                                         
vendor = input("Who's your vendor? Type it in: ")
ptype = input("Filament? Machine? Process? Type it in: ")
fullname = input("DON'T MAKE A TYPO! Name your new profile: ")
print("\"setting_id\": \"%s\"" % (quickgen(vendor, ptype, fullname)))                                                           
