
namespace InvoiceFormat {
    enum InvoiceFormatTypes { Invoice, DeliveryNote, PoisonOrder, ProformaInvoice };
}

class wpObject {
    wxString ops;
    wxString method;
    wxString sync;
    wxString searchParam;
    UniversalUniqueID linkID;
    wxString option;
    wxString returnCode;
    wxString errorMessage;
    bool isNewObject;
    bool isDeleted;

    std::vector<wxString> ftsColumn;

    bool isUpdatedRemote;   // return from update on remote server and return is OK
    bool updateFromRemote;  // request to update from remote server
    bool toUpdateRemote;    // attempt to update remote server.

    wxString md5signature;
    wxDateTime lastUpdateDateTime;
};

class PairRecord : public wpObject {
    wxString key, value;
};

class EntityRelationshipRecord : public wpObject {
    int64_t id {0};
    int64_t pairID {0};
    int64_t typeID {0};
    
    wxString remark;

};

class StockSearchParam {
    int64_t stockType;
    int64_t poisonCategory;
    int isPoison;
    int isPseudoPoison;
    int showCost;

};

class Parameter : public wpObject {
    wxString tableName;
    int64_t id {0};
    wxString intParam;
    wxString data;
    StockSearchParam stockParam;
};

class OPSTableRowRecord : public wpObject {
    wxString tableName;
    wxString keyColumnName;
    wxString keyValue;
    int64_t id {0};
    int64_t parentID {0}, 
    int64_t grandParentID {0};
    int64_t greatGrandParentID {0};
    wxString code;
    wxString name;
    wxString filterColumnName;
    wxString filterValue;
    wxString orderBy;
    wxString filterValueViaRegistry;
    std::vector<std::shared_ptr<TableDependencyRecord>> dependencies;

};


class SQLCommandRecord : public wpObject {
    wxString sql;
    wxString sqlSelect;
    wxString sqlCount;

};

class PassportRecord : public wpObject {
    int64_t id {0};
    wxString rawData;

};

class ScreenPerspectiveRecord : public wpObject {
    wxString screenName;
    wxString perspective;
    wxString centerPaneName;
};

class RequestReport : public wpObject {
    wxDateTime fromDate;
    wxDateTime toDate;
    wxString typeOfSales;

};

class RequestInfoRecord : public wpObject {
    wxString question;
    wxString answer;
    int64_t userID;
    wxDateTime fromDate;
    wxDateTime toDate;
};


class RequestForApprovalRecord : public wpObject {
    wxString uuid;
    bool asSupplier = false; 
    bool asCustomer = false;
    bool shareMembershipData = false;
    bool shareStockLevel = false;
    bool shareSalesData = false;
    wxString origin;
    wxString name;
    wxString docID;
    wxString telNo;
    wxString ref;
};

// could make it more flexible for multiple log in and out.
class AttendanceRecord : public wpObject {
    int64_t entityID;
    wxDateTime date;
    wxString ioMode;
    wxString verifyMode;
    wxString workCode;
    wxString remark;
};

class LexiconRecord : public wpObject {
    int64_t id {0};
    wxString code, name;
    int64_t parentID;
    wxString synonymID;
};

class DatabaseRecord : public LexiconRecord {
    wxString transactionName;
    wxString dbName;
    wxString scriptFolder;
    wxString wwwFolder;
};

class EntityRecord : public wpObject {
    wxString content;
    int64_t id {0};
    int64_t docID {0};
    wxString globalID;
    LexiconRecord registerAt;  // id of entityRecord
    wxDateTime registerDate;
    std::vector<std::shared_ptr<LexiconRecord>> attributes;
    std::unordered_set<int64_t> attributeSet;
};

class LocationRecord : public EntityRecord {
};

class CompanyRecord : public LocationRecord {
    double discount;
    double creditterm;
    double debitterm;
    double creditLimit;
    int isTaxable;
};

class MyKadData : public wpObject {
    wxString name
    wxString ic;
    wxString oldic;
    wxString sex;
    wxString stateOfBirth;
    wxString nationality;
    wxString race;
    wxString religion;
    wxString postcode;;
    std::vector<wxString> address;
    wxDateTime dob;
    wxDateTime validityDate;
}

class PersonRecord : public CompanyRecord {
    wxDateTime dob;
    LexiconRecord bloodType;
    LexiconRecord race;
    LexiconRecord religion;
    LexiconRecord maritalStatus;
    LexiconRecord gender;
    LexiconRecord job;;
};

class MembershipRecord : public PersonRecord {
    double pointBalance;
    wxString enrollNumber;
    std::vector<wxString> dependencyList;
};

class MembershipPointRecord : public wpObject {
    wxLongLong rowId;
    wxDateTime dateOfTransaction;
    wxString receiptNo;
    double amount;
    double points;
    double balance;
};

class AccountingRecord : public wpObject {
    wxDateTime dateOfTransaction;
    wxString refNo;
    double debit;
    double credit;
    wxString remark;
};

class TypeRecord : public LexiconRecord {
    wxString limitValue;
    wxString defaultValue;
};

class PriceCategoryRecord : public TypeRecord {
    double price;
    double localSellingPrice;
};

class TerminalRecord : public LexiconRecord {
    wxString location;
};

class UserRecord : public PersonRecord {
    std::unordered_set<long> userRoles;  // non-migratable - just ids of lexicons in roles;

    wxString encryptedPassword;
    wxString sessionID;
    wxDateTime registerDate;
    wxString menuXML;
    wxString applicationName;
    wxString schemaName;
    wxString startUpModule;
    std::unordered_set<wxString> allowedModules;

    wxString maxDiscountAllowed;
    std::vector<std::shared_ptr<LexiconRecord>> roles;
    std::vector<std::shared_ptr<LexiconRecord>> screens;
    std::unordered_set<wxString> rolesSet, screenSet;
};

class PackingRecord : public wpObject {
    int64_t stockID {0};
    LexiconRecord packType;
    wxString packDescription;
    double packSize = 1.0;
    double sellingPrice = 0.0;
    double localSellingPrice = 0.0;
    double maxDiscountPercentage = 0.0;
    double percentAboveCost = 0.0;
    std::vector<std::shared_ptr<PriceCategoryRecord>> priceByCategory;
};

class StockTaxAndCharges : public wpObject {
    TypeRecord type;
    wxDateTime fromDate;
    wxDateTime toDate;
    double percentage;
    double fixedValue;
};

class SalesPromotionRecord : public wpObject {
    wxString stockID;
    double discountPercentage = 0.0;
    wxDateTime fromDate;
    wxDateTime toDate;
};

class ComponentRecord : public wpObject {
    int64_t  stockID;
    int64_t componentID;
    PackingRecord pack;
    double quantity = 1.0;
    double costFactor = 1.0;
};

class StockRecord : public EntityRecord {
    wxString dispensingInclassion;
    wxString drugWarning;
    wxString noOfImages;
    wxString fastLookupCode;

    double reorderLevel;
    double reorderQty;
    bool isPoison;
    bool isPseudoPoison;
    bool isStockItem;
    double weightedAvgCost;  // always at the lowest sellable unit.
    double latestCost, maxCost;
    double averageCostPPOS;
    double rewardPointFactor;
    double qoh, stockValue, qohHold;
    double promotionalDiscount;
    int convertPacksizeOldData;
    wxString fcc;
    wxString displayName;

    LexiconRecord type;
    LexiconRecord defaultSellingPack;
    LexiconRecord defaultPurchasePack;
    wxString minProfitMargin;
    wxString synonymTo;
    std::vector<std::shared_ptr<PackingRecord>> packRecords;
    std::vector<std::shared_ptr<StockTaxAndCharges>> taxAndCharges;
    std::vector<std::shared_ptr<SalesPromotionRecord>> salesPromotionList;
    std::vector<std::shared_ptr<ComponentRecord>> componentList;
    std::vector<std::shared_ptr<LexiconRecord>> shelfList;
    std::vector<std::shared_ptr<PersonRecord>> salesReps;
};

class StockTransactionRecord : public StockRecord {
    wxString pack;
    wxString bonusPack;
    double quantity = 0.0;
    double bonusQty = 0.0;
    double transactionPrice = 0.0;
    double listPrice = 0.0;
    double discountAmount = 0.0;
    double srp = 0.0;
    double costValue = 0.0;
    doubletransactionValue = 0.0;
    doubletaxValue = 0.0;
    bool isSellingPriceManuallyChanged = false;  // used only for grid
    wxDateTime expiryDate;
    wxString batchNo;
    int64_t receiveDetailID;
    int64_t promoterID;
};

class HistoricalStockRecord : public StockTransactionRecord {
    int64_t transactionID;
    wxDateTime dateOfTransaction;
    wxString refNo;
    wxString qoh;  // q+l format
    wxString location;
    wxString who;
    wxString reason;
    wxString salesRep;

};

class InventoryStatusRecord : public StockRecord {
    wxString month;
    double quantityIn;
    double quantityOut;
    double quantityAdj;
    double runningTotal;
};

class QuotationRecord : public StockRecord {
    double qty;
    double qtyBonus;
    double costPrice;
    wxString qoh;
    wxString bonusScheme;
};

// historical sales record is used in association of stock record.
class HistoricalSalesRecord : public MembershipRecord {
    wxDateTime dateOfTransaction;
    wxString refNo;
    wxString pack;
    double quantity;
    double price;
    double amount;
    double bonusQty;
    wxString bonusPack;
    wxString location;
    wxString who;
};

// the Historical Purchase record is used in association with with stock record.
class HistoricalPurchaseRecord : public CompanyRecord {
    wxDateTime dateOfTransaction;
    wxString refNo;
    wxString pack;
    double quantity;
    double bonusQty;
    double price;
    double amount;
    double discount;
    double qoh;
    wxString batchNo;
    wxDateTime expiryDate;
    wxString location;
    wxString bonusPack;
    wxString qtyDisplay;
    wxString who;
    wxString transactionID;
};

class PaymentRecord : public wpObject {
    double amount;
    double tendered;
    int64_t id {0};   // paymentID
    int64_t salesID {0};  // if blank, use the SalesRecord id. By itself it should not be blank.
    wxDateTime paymentDate;
    TypeRecord paymentMethod;  // type 10 - paidBy
    wxString reference;
    CompanyRecord payor;
    double rewardPoint;
};

class GetFileParam {
    std::string remoteFilename;
};

class SearchParam : public wpObject {
    wxString search;
    wxString orderBy;
    wxString isConsumption;
    wxString isLowStock;
    wxString key;
    wxString value;
    wxString type;
    wxString method2;
    wxString option2;
    wxString column;
    int64_t id {0};

};

class StockImageRecord : public wpObject {
    wxString stockID;
    wxString imageNo;
    wxString width;
    wxString height;
};

class SalesRecord : public wpObject {
    int64_t id {0};
    wxString note;
    int seqNo {0};
    double totalAmount {0};
    double rounding{0};
    double taxValue{0};
    double discountAtTotal{0};
    double term{0};
    wxString refNo;
    wxString remark;  // reason for deleting, etc;

    MembershipRecord customer;
    CompanyRecord promoter;
    CompanyRecord deliverTo;
    UserRecord user;
    TerminalRecord terminal;

    wxDateTime transactionDate;
    TypeRecord salesType;
    std::vector<std::shared_ptr<StockTransactionRecord>> list;
    std::vector<std::shared_ptr<PaymentRecord>> paymentList;
};

class SalesRecordList : public wpObject {
    wxDateTime fromDate;
    wxDateTime toDate;
    wxString limit;
    wxString offset;
    int64_t salesTypeID;
    std::vector<std::shared_ptr<SalesRecord>> list;
};

class PurchaseRecord : public wpObject {
    int64_t id {0};
    wxString note;
    wxString refNo;
    CompanyRecord supplier;
    double term {0};
    wxString remark;
    TypeRecord purchaseType;
    wxDateTime transactionDate;
    std::vector<std::shared_ptr<StockTransactionRecord>> list;
    std::vector<std::shared_ptr<PaymentRecord>> paymentList;
};

class PurchaseRecordList : public wpObject {
    wxDateTime fromDate;
    wxDateTime toDate;
    wxString limit;
    wxString offset;
    std::vector<std::shared_ptr<PurchaseRecord>> list;
};

class AccountJournalRecord : public EntityRecord {
    wxString invoice;
    wxDateTime date;
    double invAmount;
    double payAmount;
    wxString remark;
};

class AccountTransactionRecord : public wpObject {
    int64_t id {0};
    int seqNo;
    double totalAmount;
    wxString note;
    wxString refNo;
    wxString remark;
    wxDateTime transactionDate;
    EntityRecord entityFrom;
    EntityRecord entityTo;
    wxString type;  // out or in
    std::vector<std::shared_ptr<AccountJournalRecord>> list;
};

class AccountTransactionRecordList : public wpObject {
    wxDateTime fromDate, toDate;
    wxString limit, offset;
    std::vector<std::shared_ptr<AccountTransactionRecord>> list;
};

class SyncRecord : public wpObject {
    wxDateTime syncDate;
    int64_t id {0};
    RecordList list;
};

class MessageLayer {
    int layer;  // for receiptPrinter layer=1 for Header; layer=2 for Footer
        // for customerDisplay layer=n for each page of the display ???
    std::vector<wxString> messages;
};

class MessageRecord : public wpObject {
    int64_t id {0};
    wxDateTime effectiveDate;   // for searching
    LexiconRecord messageType;  // 1=receiptPrinter, 2=customerDisplay
    wxString priority;
    wxDateTime fromDate;
    wxDateTime toDate;
    std::vector<std::shared_ptr<MessageLayer>> messages;
};

class ServerEventMessage : public wpObject {
    class MessageRecord {
        wxString code;
        wxString key, value;
    };
    int type;
    bool isBroadcast;
    bool isMobileNotification;
    std::vector<std::shared_ptr<MessageRecord>> messages;
};

class ServerKeyRecord : public wpObject {
    std::unordered_map<wxString, int*> map;
    std::vector<StringPair> pointValueWeightList;
    MessageRecord customerDisplay;
    MessageRecord printer;
    UniversalUniqueID entityServerID;
    UniversalUniqueID serverGroupID;
    UniversalUniqueID serverUUID;
    bool adminCanModifyQOH;
    bool allowDuplicateCode;
    bool autoCodeCheckPrefix;
    bool autoGenerateItemCode;
    bool autoGenerateMembershipCode;
    bool autoGenerateSupplierCode;
    bool doRunHereIfServerNotFound;  // run command here if not found
    bool enableCheckPoisonSales;
    bool enableCheckPseudoPoisonSales;
    bool enableCreditLimitCheck;
    bool enableFractionalPackSize;
    bool enableFractionalQuantity;
    bool hasGST;
    bool invoiceFormatPortrait;
    bool invoiceShowCustomerAllPages;  // show customer name in all invoice pages;
    int noOfDecimalPointForPrice;
    bool isAutoComputeDefaultPrices;
    bool isAutoComputeOtherPackPrices;
    bool isPriceIncludeTax;
    bool isTwoLineDescOnReceipt;
    bool movementIncludeHold;
    bool printReceiptByDefault;
    bool qohIncludeHoldIn;
    bool qohIncludeHoldOut;
    bool qrBeforeGoodBye;
    bool retainUserIDonRelogin;  //1 == yes;
    bool roundToNearest5cents;
    bool enableDifferentDateTransaction = false;
    bool roundToNearest5cents_wholesales;
    bool runLocalSearch;
    bool runScriptInDbLocation;  // run all script root folder in the same as current DB folder
    bool saveStockPackPrices;    // to save price by category in StockPackPrices - meaning that this will detach the price for edited item from stockpricecategory factorAboveCost
    bool showCategoryInSalesSummaryPDF;
    bool showCategoryInSalesSummaryReceiptPrinter;
    bool showDiscountOnWholesalesInvoice;
    bool showDiscountPerItemOnReceipt;
    bool showListAllCases = false;
    bool showGSTSummary;
    bool showMALonInvoice = true;
    bool showBatchExpiryOnInvoice = true;
    bool showRewardPoints;
    bool showTotalDiscountOnReceipt;
    bool truncateSum;
    bool useLocalPrice;
    bool useRMOnlyForPoints;  // no cents
    bool whsUseCost;          // use cost price in wholesales
    double RMtoOnePoint;
    double pointsToOneRM;  //=200 points to one RM
    double qrImageFactor;  // 1=full page, 0.5, half
    int ageActiveInMonths;
    int allowEditInvoiceRecord;
    int autoItemCodeLength;
    int autoMembershipCodeLength;
    int autoSupplierCodeLength;
    int defaultSalesSmallestPack;  // 1==yes;
    int enableAccounting;
    int enableBackup;
    int enableCatering;
    int enableDispensingLabel;
    int enableHR;
    int enableMyKad;
    int enableOnlineStore;
    int enablePatientRecord;
    int enablePrintLabel;
    int enableRental;
    int enableTicketing;
    int idleTimeToLockScreen;
    int idleTimeToStartDemo;
    int noOfLinesEject;
    int percentMarginAboveCost;
    int receiptPrinterPointSize;
    int receiptPrinterSpacing;
    int sumToNearest;
    wxDateTime softwareExpiryDate;
    wxDateTime softwareRegisterDate;
    wxDateTime::WeekFlags weekflag;
    int64_t IBTTypeID;
    wxString membershipRewardStoreCode;
    int64_t accBankTypeGroupID;
    int64_t accCashTypeGroupID;
    int64_t accJournalTypeExpenses;
    int64_t accJournalTypeOthers;
    int64_t accJournalTypePurchases;
    int64_t accJournalTypeSales;
    int64_t adjustStockTypeID;
    wxString applicationName;
    wxString applicationType;
    int64_t caraBayarGroupID;
    int64_t cashSalesTypeID;
    wxString categoryListCrossTab;  // list of category names separated by ';' for crosstab, those not in this list will be grouped into OTHERS
    int64_t creditSalesTypeID;
    wxString defaultColumnForZeroCostInSales;  // latestCost, weightedAvgCost, maxCost
    wxString defaultServiceChargePercentage;
    int64_t entityTypeCustomer;
    int64_t entityTypeGroupID;
    int64_t entityTypeMember;
    int64_t entityTypePromoter;
    int64_t entityTypeSupplier;
    wxString firstDayOfWeek;
    wxString gstMarker;  // S
    wxString gstRegNo;
    wxString gstZeroMarker;       // Z
    wxString invoice_Disclaimer;  // print on invoice
    wxString osVersion;
    wxString outletAddress;
    wxString outletCode;
    wxString outletName;
    wxString outletRegNo;
    int64_t payByAmex;
    int64_t payByCash;
    int64_t payByCheque;
    int64_t payByCredit;
    int64_t payByMasterCard;
    int64_t payByRewardPoint;
    int64_t payByVisa;
    int64_t payByVoucher;
    wxString poisonCategoryMustControl;
    int64_t poisonCategoryTypeGroupID;
    wxString poisonOrderNote;
    int64_t priceCategoryGroupID;
    wxString qrString;
    wxString quotation_Disclaimer;          // print on quotation
    wxString referenceCostForSellingPrice;  // max, latest, average
    int64_t stockTransactionTypeGroupID;
    int64_t stockTypeGroupID;
    int64_t taxGSTTypeID;
    wxString taxPercentage;
    wxString versionString;

};

class LabelInputParam : public wpObject {
    wxString label;
    wxString type;
    wxString param;
    wxString value;
    wxString startFlag;
    wxString lookupOps;
    wxString list;
    wxString defaultValue;
    int useTimeAsItIs;
};

class MenuDialogParam : public wpObject {
    wxString title;
    std::vector<std::shared_ptr<LabelInputParam>> paramList;
};

class MenuActionParam : public wpObject {
    wxString name;
    wxString scriptName;
    wxString outputType;
    wxString orientation;
    wxString cmdBefore;
    wxString cmdAfter;
    wxString userParam;
    wxString sqlLink;
    wxString sheetType;
    MenuDialogParam dialog;
};

class ServerInfo : public wpObject {
    wxString outletName;
    wxString outletCode;
    UniversalUniqueID serverUUID;
    UniversalUniqueID groupID;
    wxLongLong connectionCtx;
    UniversalUniqueID webServerID;
};


class LoginSessionRecord : public wpObject {
    std::vector<std::shared_ptr<DatabaseRecord>> databaseList;
    std::vector<std::shared_ptr<ServerInfo>> otherServers;

    DatabaseRecord databaseSelected;
    TerminalRecord terminal;
    wxString serverUUID;
    wxString groupID;
    wxString entityServerUUID;
    wxString peerID;
    UserRecord user;

    bool isHTTP = false;
    bool isExternalSocket = false;
    int isCallBack;
    int listeningPortNo;
    int needUpdate;
    long portNo;
    int serviceNo;
    uintptr_t connectionCtx;
    wxDateTime expiryDate;
    wxDateTime timeIn;
    wxDateTime timeOut;
    int64_t id = 0;  // sessionID
    wxMutex isProcessing;
    wxString applicationName;
    wxString certificate;
    wxString connectFromAddress;
    wxString ipAddress;
    wxString outletCode;
    wxString outletName;
    wxString remark;
    wxString serverName;
    wxString serverOsVersion;
    wxString serverAppVersion;
    wxString clientOsVersion;
    wxString clientAppVersion;
    wxString sessionKey;
    wxString systemUser;
    wxString fileToTransmit;
    bool isAserver = true;
    bool isLiveConnection = false;

};

class WorkShiftRecord : public wpObject {
    int64_t id {0};
    wxDateTime timeEnd;
    wxDateTime timeStart;
    LoginSessionRecord closeBy;
    LoginSessionRecord createdBy;
    double closingBalance;
    double openBalance;
};

class ActivateSoftwareRecord : public wpObject {
    int64_t accounting;
    int64_t ticketing;
    int64_t catering;
    int64_t onlineStore;
    int64_t rental;
    int64_t stockTracking;
    int64_t mykad;
    int64_t backup;
    int64_t softwareExpiry;
    int64_t dispensing;
};

class AuthenticationRecord : public wpObject {
    int64_t id {0};
    wxString userid;
    wxString password;
    wxString email;
    wxString telephone;
    wxString fbId;
    wxString googleId;
    wxString token;
    wxString message;
};

class RegisterSoftwareRecord : public wpObject {
    int64_t id {0};
    wxString uuid;
    wxString groupID;
    wxString entityUUID;
    wxString userName;
    wxString password;
    wxString email;
    wxString telephone;
    wxString outletName;
    wxString address;
    wxString cidHash;
    wxString dbFolder;
    wxString dbTransFolder;
    wxString version;
    class ActivationRecord {
        wxString accounting;
        wxString ticketing;
        wxString catering;
        wxString onlineStore;
        wxString rental;
        wxString stockTracking;
        wxString mykad;
        wxString backup;
        wxString softwareExpiry;
        wxString dispensing;
        wxString patientRecord;
        wxString hr;
    };
    ActivationRecord activationKey;
    wxDateTime registrationDate;
    wxDateTime expiryDate;
    wxDateTime lastLoginDate;
    wxString ulKeys;
    wxString ulLocalKeys;
    wxString messages;
    wxString messageDetail;
    wxString outletCode;

};